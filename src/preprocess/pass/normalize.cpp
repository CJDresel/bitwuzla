#include "preprocess/pass/normalize.h"

#include <cmath>

#include "env.h"
#include "node/node_manager.h"
#include "node/node_ref_vector.h"
#include "node/node_utils.h"
#include "node/unordered_node_ref_map.h"
#include "node/unordered_node_ref_set.h"

namespace bzla::preprocess::pass {

using namespace bzla::node;

/* --- PassNormalize public ------------------------------------------------- */

PassNormalize::PassNormalize(Env& env,
                             backtrack::BacktrackManager* backtrack_mgr)
    : PreprocessingPass(env, backtrack_mgr), d_stats(env.statistics())
{
}

namespace {
bool
is_leaf(Kind kind,
        const Node& node,
        const std::unordered_map<Node, uint64_t>& parents,
        const std::unordered_map<Node, uint64_t>& parents_in_chain)
{
  if (node.kind() != kind)
  {
    return true;
  }
  auto p = parents.find(node);
  if (p == parents.end()) return false;
  auto pp = parents_in_chain.find(node);
  if (pp == parents_in_chain.end()) return false;
  return pp->second < p->second;
}
}  // namespace

std::unordered_map<Node, BitVector>
PassNormalize::compute_factors(
    const Node& node, const std::unordered_map<Node, uint64_t>& parents)
{
  bool share_aware = !parents.empty();
  Kind kind        = node.kind();
  BitVector zero   = BitVector::mk_zero(node.type().bv_size());

  node_ref_vector nodes;
  unordered_node_ref_set intermediate;
  unordered_node_ref_map<BitVector> coeffs;
  std::unordered_map<Node, BitVector> res;      // only leafs

  // Collect all traversed nodes (intermediate nodes of specified kind and
  // leafs) and initialize coefficients for each node to zero.
  node_ref_vector visit{node};
  do
  {
    const Node& cur     = visit.back();
    auto [it, inserted] = coeffs.emplace(cur, zero);
    visit.pop_back();
    if (inserted)
    {
      nodes.push_back(cur);
      if (cur.kind() == kind)
      {
        if (share_aware)
        {
          // treat as leaf if node of given kind has parent references
          // from outside the current 'kind' chain
          assert(d_parents.find(cur) != d_parents.end());
          assert(parents.find(cur) != parents.end());
          if (is_leaf(kind, cur, d_parents, parents))
          {
            continue;
          }
        }
        intermediate.insert(cur);
        visit.insert(visit.end(), cur.begin(), cur.end());
      }
    }
  } while (!visit.empty());

  // Compute leaf coefficients by pushing initial top node coefficient to leafs.
  //
  // Note: We have to ensure that parents are fully processed before we compute
  //       the coefficient for its children. Hence, we sort the nodes in
  //       ascending order and process the nodes with the higher IDs first.
  std::sort(nodes.begin(), nodes.end());
  assert(nodes.back() == node);
  coeffs[node].ibvinc();  // Set initial coefficient of top node
  for (auto it = nodes.rbegin(), rend = nodes.rend(); it != rend; ++it)
  {
    const Node& cur = *it;
    auto fit        = coeffs.find(cur);
    assert(fit != coeffs.end());

    // If it's an intermediate node, push factor down to children
    if (intermediate.find(cur) != intermediate.end())
    {
      assert(cur.kind() == kind);
      for (const auto& child : cur)
      {
        assert(coeffs.find(child) != coeffs.end());
        coeffs[child].ibvadd(fit->second);
      }
    }
    // If it's a leaf, just copy the result
    else
    {
      assert(res.find(cur) == res.end());
      res.emplace(cur, coeffs[cur]);
    }
  }
  return res;
}

namespace {
std::unordered_map<Node, uint64_t>
_count_parents(const node_ref_vector& nodes, Kind kind)
{
  std::unordered_map<Node, uint64_t> parents;
  node::unordered_node_ref_set cache;
  for (size_t i = 0, size = nodes.size(); i < size; ++i)
  {
    node::node_ref_vector visit{nodes[i]};
    parents.emplace(nodes[i], 1);
    do
    {
      const Node& cur     = visit.back();
      auto [it, inserted] = cache.insert(cur);
      visit.pop_back();
      if (inserted
          && (cur.kind() == kind
              || (kind == Kind::BV_ADD && cur.is_inverted()
                  && cur[0].kind() == kind)))
      {
        for (auto& child : cur)
        {
          parents[child] += 1;
          visit.push_back(child);
        }
      }
    } while (!visit.empty());
  }
  return parents;
}

bool
_normalize_factors_eq_add(std::unordered_map<Node, BitVector>& factors0,
                          std::unordered_map<Node, BitVector>& factors1,
                          uint64_t bv_size)
{
  // Note: Factors must already be normalized in the sense that they only
  //       either appear on the left or right hand side (this function must
  //       be called with factors determined by get_normalized_factors()).

  // (a - b + c = -d + e) is normalized to (a + c + d = b + e)

  // ~x = ~(x + 1) + 1
  // -x = ~x + 1

  NodeManager& nm = NodeManager::get();
  Node one        = nm.mk_value(BitVector::mk_one(bv_size));
  BitVector bvzero = BitVector::mk_zero(bv_size);
  bool normalized = false;

  // summarize values
  BitVector lvalue = BitVector::mk_zero(bv_size);
  for (auto& f : factors0)
  {
    if (f.first.is_value())
    {
      lvalue.ibvadd(f.first.value<BitVector>().bvmul(f.second));
      f.second   = bvzero;
      normalized = true;
    }
  }

  // move negated occurrences to other side
  for (auto& f : factors0)
  {
    const Node& cur = f.first;
    BitVector factor = f.second;
    if (factor.is_zero())
    {
      continue;
    }
    if (cur.is_inverted())
    {
      Node neg;
      if (cur[0].kind() == Kind::BV_ADD)
      {
        if (cur[0][0] == one)
        {
          neg = cur[0][1];
          lvalue.ibvsub(factor);
          f.second = bvzero;
        }
        else if (cur[0][1] == one)
        {
          neg = cur[0][0];
          lvalue.ibvsub(factor);
          f.second = bvzero;
        }
      }
      else
      {
        neg = cur[0];
        f.second = bvzero;
      }
      if (!neg.is_null())
      {
        normalized = true;
        auto it    = factors1.find(neg);
        if (it == factors1.end())
        {
          factors1.emplace(neg, factor);
        }
        else
        {
          it->second.ibvadd(factor);
        }
        lvalue.ibvsub(factor);
      }
    }
  }
  if (!lvalue.is_zero())
  {
    Node val = nm.mk_value(lvalue);
    auto it  = factors0.find(val);
    if (it == factors0.end())
    {
      factors0.emplace(val, BitVector::mk_one(bv_size));
    }
    else
    {
      it->second.ibvinc();
    }
  }

  return normalized;
}
}  // namespace

std::tuple<std::unordered_map<Node, BitVector>,
           std::unordered_map<Node, BitVector>,
           bool>
PassNormalize::get_normalized_factors_for_eq(const Node& node0,
                                             const Node& node1,
                                             bool share_aware)
{
  assert(node0.kind() == node1.kind());
  bool normalized = false;
  Kind kind       = node0.kind();
  std::unordered_map<Node, uint64_t> parents =
      share_aware ? _count_parents({node0, node1}, kind)
                  : std::unordered_map<Node, uint64_t>();
  auto factors0 = compute_factors(node0, parents);
  auto factors1 = compute_factors(node1, parents);
  std::unordered_map<Node, BitVector> res0, res1;
  // normalize common factors and record entries that are not in factors1
  for (const auto& f : factors0)
  {
    assert(f.first.kind() != kind
           || (share_aware && parents.at(f.first) != d_parents.at(f.first)));

    auto fit = factors1.find(f.first);
    if (fit == factors1.end())
    {
      res0.insert(f);
      continue;
    }
    if (f.second == fit->second) continue;
    if (f.second.compare(fit->second) > 0)
    {
      assert(res0.find(f.first) == res0.end());
      res0.emplace(f.first, f.second.bvsub(fit->second));
      normalized = true;
    }
    else
    {
      assert(res1.find(f.first) == res1.end());
      res1.emplace(f.first, fit->second.bvsub(f.second));
      normalized = true;
    }
  }
  // check factors1 for entries that are not in factors0
  for (const auto& f : factors1)
  {
    assert(f.first.kind() != kind
           || (share_aware && parents.at(f.first) != d_parents.at(f.first)));
    auto fit = factors0.find(f.first);
    if (fit == factors0.end())
    {
      res1.insert(f);
    }
  }

  // factors on both sides cancelled out
  if (res0.empty() && res1.empty())
  {
    return {{}, {}, true};
  }

  if (kind == Kind::BV_ADD)
  {
    uint64_t bv_size = node0.type().bv_size();
    if (_normalize_factors_eq_add(res0, res1, bv_size) && !normalized)
    {
      normalized = true;
    }
    if (_normalize_factors_eq_add(res1, res0, bv_size) && !normalized)
    {
      normalized = true;
    }
  }

  return {res0, res1, normalized};
}

std::pair<Node, Node>
PassNormalize::_normalize_eq_mul(
    const std::unordered_map<Node, BitVector>& factors0,
    const std::unordered_map<Node, BitVector>& factors1,
    uint64_t bv_size)
{
  NodeManager& nm = NodeManager::get();
  std::vector<Node> lhs, rhs;
  if (factors0.empty())
  {
    lhs.push_back(nm.mk_value(BitVector::mk_one(bv_size)));
  }
  else
  {
    for (const auto& f : factors0)
    {
      if (f.second.is_zero())
      {
        continue;
      }
      assert(BitVector::fits_in_size(64, f.second.str(), 2));
      lhs.insert(lhs.end(), f.second.to_uint64(true), f.first);
    }
  }
  if (factors1.empty())
  {
    rhs.push_back(nm.mk_value(BitVector::mk_one(bv_size)));
  }
  else
  {
    for (const auto& f : factors1)
    {
      if (f.second.is_zero())
      {
        continue;
      }
      assert(BitVector::fits_in_size(64, f.second.str(), 2));
      rhs.insert(rhs.end(), f.second.to_uint64(true), f.first);
    }
  }
  assert(!lhs.empty() && !rhs.empty());

  std::sort(lhs.begin(), lhs.end());
  std::sort(rhs.begin(), rhs.end());

  Node left, right;
  size_t n = lhs.size();
  left     = lhs[n - 1];
  for (size_t i = 1; i < n; ++i)
  {
    left = nm.mk_node(Kind::BV_MUL, {lhs[n - i - 1], left});
  }
  n = rhs.size();
  right = rhs[n - 1];
  for (size_t i = 1; i < n; ++i)
  {
    right = nm.mk_node(Kind::BV_MUL, {rhs[n - i - 1], right});
  }
  return {left, right};
}

namespace {
Node
get_factorized_add(const Node& node, const BitVector& factor)
{
  assert(!node.is_null());
  NodeManager& nm = NodeManager::get();
  assert(!factor.is_zero());
  if (factor.is_one())
  {
    return node;
  }
  if (factor.is_ones())
  {
    return nm.mk_node(Kind::BV_NEG, {node});
  }
  return nm.mk_node(Kind::BV_MUL, {nm.mk_value(factor), node});
}
}  // namespace

std::pair<Node, Node>
PassNormalize::_normalize_eq_add(std::unordered_map<Node, BitVector>& factors0,
                                 std::unordered_map<Node, BitVector>& factors1,
                                 uint64_t bv_size)
{
  NodeManager& nm = NodeManager::get();

  BitVector lvalue = BitVector::mk_zero(bv_size);
  BitVector rvalue = BitVector::mk_zero(bv_size);

  std::vector<Node> lhs, rhs;

  for (const auto& f : factors0)
  {
    const Node& cur         = f.first;
    const BitVector& factor = f.second;
    if (factor.is_zero())
    {
      continue;
    }
    if (cur.is_value())
    {
      lvalue.ibvadd(factor.bvmul(cur.value<BitVector>()));
    }
    else
    {
      lhs.push_back(get_factorized_add(cur, factor));
    }
  }
  for (const auto& f : factors1)
  {
    const Node& cur         = f.first;
    const BitVector& factor = f.second;
    if (factor.is_zero())
    {
      continue;
    }
    if (cur.is_value())
    {
      rvalue.ibvadd(factor.bvmul(cur.value<BitVector>()));
    }
    else
    {
      rhs.push_back(get_factorized_add(cur, factor));
    }
  }

  // normalize values, e.g., (a + 2 = b + 3) -> (a - 1 = b)
  if (!lvalue.is_zero())
  {
    lvalue.ibvsub(rvalue);
    if (!lvalue.is_zero())
    {
      lhs.push_back(nm.mk_value(lvalue));
    }
  }
  else if (!rvalue.is_zero())
  {
    rhs.push_back(nm.mk_value(rvalue));
  }

  std::sort(lhs.begin(), lhs.end());
  std::sort(rhs.begin(), rhs.end());

  Node left  = lhs.empty() ? nm.mk_value(BitVector::mk_zero(bv_size))
                           : node::utils::mk_nary(Kind::BV_ADD, lhs);
  Node right = rhs.empty() ? nm.mk_value(BitVector::mk_zero(bv_size))
                           : node::utils::mk_nary(Kind::BV_ADD, rhs);
  return {left, right};
}

std::pair<Node, bool>
PassNormalize::normalize_eq_add_mul(const Node& node0,
                                    const Node& node1,
                                    bool share_aware)
{
  assert(node0.kind() == node1.kind());
  assert(node0.kind() == Kind::BV_MUL || node0.kind() == Kind::BV_ADD);

  NodeManager& nm = NodeManager::get();

  auto [factors0, factors1, normalized] =
      get_normalized_factors_for_eq(node0, node1, share_aware);

  if (!normalized)
  {
    return {nm.mk_node(Kind::EQUAL, {node0, node1}), false};
  }

  if (factors0.empty() && factors1.empty())
  {
    return {nm.mk_value(true), true};
  }

  auto [left, right] =
      node0.kind() == Kind::BV_ADD
          ? _normalize_eq_add(factors0, factors1, node0.type().bv_size())
          : _normalize_eq_mul(factors0, factors1, node0.type().bv_size());

  return {nm.mk_node(Kind::EQUAL, {left, right}), true};
}

namespace {

void
push_factorized(Kind kind,
                const Node& node,
                const BitVector& occs,
                std::vector<Node>& nodes)
{
  if (kind == Kind::BV_ADD)
  {
    if (!occs.is_zero())
    {
      NodeManager& nm = NodeManager::get();
      nodes.push_back(nm.mk_node(Kind::BV_MUL, {nm.mk_value(occs), node}));
    }
  }
  else
  {
    assert(kind == Kind::BV_MUL || kind == Kind::BV_XOR
           || kind == Kind::BV_AND);
    assert(BitVector::fits_in_size(64, occs.str(), 2));
    nodes.insert(nodes.end(), occs.to_uint64(true), node);
  }
}

}  // namespace

std::pair<Node, Node>
PassNormalize::normalize_common(Kind kind,
                                std::unordered_map<Node, BitVector>& lhs,
                                std::unordered_map<Node, BitVector>& rhs)
{
  std::vector<Node> lhs_norm, rhs_norm, common;
  assert(!lhs.empty());
  assert(!rhs.empty());

  BitVector one = BitVector::mk_one(lhs.begin()->first.type().bv_size());

  if (kind == Kind::BV_AND)
  {
    for (auto it = lhs.begin(), end = lhs.end(); it != end; ++it)
    {
      if (it->second.compare(one) > 0)
      {
        it->second = one;
      }
    }
    for (auto it = rhs.begin(), end = rhs.end(); it != end; ++it)
    {
      if (it->second.compare(one) > 0)
      {
        it->second = one;
      }
    }
  }

  for (auto it0 = lhs.begin(), end = lhs.end(); it0 != end; ++it0)
  {
    auto it1 = rhs.find(it0->first);
    if (it1 != rhs.end())
    {
      auto occs =
          it0->second.compare(it1->second) <= 0 ? it0->second : it1->second;
      if (occs.is_zero())
      {
        continue;
      }
      it0->second.ibvsub(occs);
      it1->second.ibvsub(occs);
      push_factorized(kind, it0->first, occs, common);
    }
  }

  for (const auto& [n, occs] : lhs)
  {
    push_factorized(kind, n, occs, lhs_norm);
  }
  for (const auto& [n, occs] : rhs)
  {
    push_factorized(kind, n, occs, rhs_norm);
  }

  if (!common.empty())
  {
    std::sort(common.begin(), common.end());
    Node com = utils::mk_nary(kind, common);
    lhs_norm.push_back(com);
    rhs_norm.push_back(com);
  }

  assert(!lhs_norm.empty());
  assert(!rhs_norm.empty());

  std::sort(lhs_norm.begin(), lhs_norm.end());
  std::sort(rhs_norm.begin(), rhs_norm.end());

  Node left  = utils::mk_nary(kind, lhs_norm);
  Node right = utils::mk_nary(kind, rhs_norm);

  return {left, right};
}

std::pair<Node, bool>
PassNormalize::normalize_comm_assoc(Kind parent_kind,
                                    const Node& node0,
                                    const Node& node1,
                                    bool share_aware)
{
  NodeManager& nm = NodeManager::get();

  Node top_lhs = get_top(node0);
  Node top_rhs = get_top(node1);

  Kind kind = top_lhs.kind();
  if (kind != top_rhs.kind()
      || (kind != Kind::BV_ADD && kind != Kind::BV_MUL))
    // && kind != Kind::BV_AND && kind != Kind::BV_XOR))
  {
    return {nm.mk_node(parent_kind, {node0, node1}), false};
  }

  // Note: parents could also be computed based on node0 and node1, but
  //       get_top() and rebuild_top() do not handle this case yet.
  std::unordered_map<Node, uint64_t> parents =
      share_aware ? _count_parents({top_lhs, top_rhs}, kind)
                  : std::unordered_map<Node, uint64_t>();

  auto lhs           = compute_factors(top_lhs, parents);
  auto rhs           = compute_factors(top_rhs, parents);
  auto [left, right] = normalize_common(kind, lhs, rhs);
  auto rebuilt_left  = rebuild_top(node0, top_lhs, left);
  auto rebuilt_right = rebuild_top(node1, top_rhs, right);
  return {nm.mk_node(parent_kind, {rebuilt_left, rebuilt_right}), false};
}

Node
PassNormalize::get_top(const Node& node)
{
  Node cur = node;
  Kind k;
  while (true)
  {
    k = cur.kind();
    if (k == Kind::BV_NOT || k == Kind::BV_SHL || k == Kind::BV_SHR
        || k == Kind::BV_EXTRACT)
    {
      cur = cur[0];
    }
    else
    {
      break;
    }
  }
  return cur;
}

Node
PassNormalize::rebuild_top(const Node& node,
                           const Node& top,
                           const Node& normalized)
{
  (void) top;

  node_ref_vector visit{node};
  std::unordered_map<Node, Node> cache;

  Kind k;
  do
  {
    const Node& cur = visit.back();

    auto [it, inserted] = cache.emplace(cur, Node());

    if (inserted)
    {
      k = cur.kind();
      if (k == Kind::BV_NOT || k == Kind::BV_SHL || k == Kind::BV_SHR
          || k == Kind::BV_EXTRACT)
      {
        visit.push_back(cur[0]);
        // Other children stay the same
        for (size_t i = 1, size = cur.num_children(); i < size; ++i)
        {
          cache.emplace(cur[i], cur[i]);
        }
        continue;
      }
      else
      {
        assert(cur == top);
        it->second = normalized;
      }
    }
    else if (it->second.is_null())
    {
      it->second = utils::rebuild_node(cur, cache);
    }
    visit.pop_back();
  } while (!visit.empty());
  return cache.at(node);
}

void
PassNormalize::apply(AssertionVector& assertions)
{
  util::Timer timer(d_stats.time_apply);

  d_cache.clear();
  assert(d_parents.empty());
  for (size_t i = 0, size = assertions.size(); i < size; ++i)
  {
    count_parents(assertions[i], d_parents, d_parents_cache);
  }

  for (size_t i = 0, size = assertions.size(); i < size; ++i)
  {
    const Node& assertion = assertions[i];
    if (!processed(assertion))
    {
      const Node& processed = process(assertion);
      assertions.replace(i, processed);
      cache_assertion(processed);
    }
  }
  d_parents.clear();
  d_parents_cache.clear();
  d_cache.clear();
}

Node
PassNormalize::process(const Node& node)
{
  bool share_aware = d_env.options().pp_normalize_share_aware();
  node_ref_vector visit{node};
  do
  {
    const Node& cur     = visit.back();
    auto [it, inserted] = d_cache.emplace(cur, Node());
    if (inserted)
    {
      visit.insert(visit.end(), cur.begin(), cur.end());
      continue;
    }
    else if (it->second.is_null())
    {
      std::vector<Node> children;
      for (const Node& child : cur)
      {
        auto itc = d_cache.find(child);
        assert(itc != d_cache.end());
        assert(!itc->second.is_null());
        children.push_back(itc->second);
      }

      Kind k = cur.kind();
      if (k == Kind::EQUAL && children[0].kind() == children[1].kind()
          && (children[0].kind() == Kind::BV_ADD
              || children[0].kind() == Kind::BV_MUL))
      {
        auto [res, normalized] =
            normalize_eq_add_mul(children[0], children[1], share_aware);
        it->second = res;
        if (normalized) d_stats.num_normalizations += 1;
      }
      else if (k == Kind::EQUAL || k == Kind::BV_ULT || k == Kind::BV_SLT)
      {
        auto [res, normalized] =
            normalize_comm_assoc(k, children[0], children[1], share_aware);
        it->second = res;
        if (normalized) d_stats.num_normalizations += 1;
      }
#if 0
      else if ((k == Kind::BV_AND || k == Kind::BV_ADD || k == Kind::BV_MUL)
               && (cur[0].kind() == cur[1].kind() && cur[0].kind() != k))
      {
        auto [res, normalized] =
            normalize_comm_assoc(k, children[0], children[1], share_aware);
        it->second = res;
        if (normalized) d_stats.num_normalizations += 1;
      }
#endif
      else
      {
        it->second = node::utils::rebuild_node(cur, children);
      }

      // Count parents for newly constructed nodes
      count_parents(it->second, d_parents, d_parents_cache);
    }
    visit.pop_back();
  } while (!visit.empty());
  auto it = d_cache.find(node);
  assert(it != d_cache.end());
  return d_env.rewriter().rewrite(it->second);
}

/* --- PassNormalize private ------------------------------------------------ */

PassNormalize::Statistics::Statistics(util::Statistics& stats)
    : time_apply(stats.new_stat<util::TimerStatistic>(
        "preprocess::normalize::time_apply")),
      num_normalizations(
          stats.new_stat<uint64_t>("preprocess::normalize::num_normalizations"))
{
}

/* -------------------------------------------------------------------------- */
}  // namespace bzla::preprocess::pass
