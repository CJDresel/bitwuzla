#include "bb/aig_printer.h"

#include <sstream>
#include <vector>

#include "bb/aigmgr.h"

namespace bzla::bb::aig {

void
Smt2Printer::print(std::stringstream& ss, const AigNode& n)
{
  bool negate = n.is_negated() && !n.is_true() && !n.is_false();
  if (negate)
  {
    ss << "(bvnot ";
  }
  if (n.is_false())
  {
    ss << "#b0";
  }
  else if (n.is_true())
  {
    ss << "#b1";
  }
  else if (n.is_const())
  {
    ss << "x" << std::labs(n.get_id());
  }
  else
  {
    assert(n.is_and());
    ss << "a" << std::labs(n.get_id());
  }
  if (negate)
  {
    ss << ")";
  }
}

void
Smt2Printer::print(std::stringstream& ss, const std::vector<AigNode>& bits)
{
  std::vector<AigNode> visit{bits.begin(), bits.end()};
  std::unordered_map<int64_t, bool> cache;

  do
  {
    AigNode n = visit.back();
    visit.pop_back();
    uint64_t id = std::labs(n.get_id());

    auto it = cache.find(id);
    if (it == cache.end())
    {
      cache.emplace(id, false);

      if (n.is_and())
      {
        visit.push_back(n);
        visit.push_back(n[0]);
        visit.push_back(n[1]);
      }
    }
    else if (!it->second)
    {
      it->second = true;
      if (n.is_and())
      {
        ss << "(define-fun a" << id << "() (_ BitVec 1) ";

        ss << "(bvand ";
        print(ss, n[0]);
        ss << " ";
        print(ss, n[1]);
        ss << ")";
        ss << ")\n";
      }
    }
  } while (!visit.empty());
}

}  // namespace bzla::bb::aig
