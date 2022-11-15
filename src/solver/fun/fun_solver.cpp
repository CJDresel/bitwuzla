#include "solver/fun/fun_solver.h"

#include "node/node_manager.h"
#include "node/node_utils.h"
#include "solver/solver_engine.h"

namespace bzla::fun {

using namespace node;

/* --- FunSolver public ----------------------------------------------------- */

bool
FunSolver::is_leaf(const Node& term)
{
  Kind k = term.kind();
  return k == Kind::APPLY || (k == Kind::EQUAL && (term[0].type().is_fun()));
}

FunSolver::FunSolver(SolverEngine& solver_engine)
    : Solver(solver_engine), d_applies(solver_engine.backtrack_mgr())
{
}

FunSolver::~FunSolver() {}

void
FunSolver::check()
{
  d_fun_models.clear();
  for (const Node& apply : d_applies)
  {
    const Node& fun = apply[0];
    auto& fun_model = d_fun_models[fun];

    Apply a(apply, d_solver_engine);
    auto [it, inserted] = fun_model.insert(a);
    if (!inserted)
    {
      // Function congruence conflict
      if (it->value() != a.value())
      {
        add_function_congruence_lemma(apply, it->get());
      }
    }
  }
}

Node
FunSolver::value(const Node& term)
{
  assert(term.type().is_fun());
  return Node();
}

void
FunSolver::register_term(const Node& term)
{
  // For now we only expect function applications
  assert(term.kind() == Kind::APPLY);
  d_applies.push_back(term);
}

/* --- FunSolver private ---------------------------------------------------- */

void
FunSolver::add_function_congruence_lemma(const Node& a, const Node& b)
{
  assert(a.num_children() == b.num_children());
  assert(a.kind() == Kind::APPLY);
  assert(b.kind() == Kind::APPLY);

  NodeManager& nm = NodeManager::get();
  std::vector<Node> premise;
  for (size_t i = 1, size = a.num_children(); i < size; ++i)
  {
    premise.emplace_back(nm.mk_node(Kind::EQUAL, {a[i], b[i]}));
  }
  Node conclusion = nm.mk_node(Kind::EQUAL, {a, b});
  Node lemma      = nm.mk_node(Kind::IMPLIES,
                          {utils::mk_nary(Kind::AND, premise), conclusion});
  d_solver_engine.lemma(lemma);
}

/* --- Apply public --------------------------------------------------------- */

FunSolver::Apply::Apply(const Node& apply, SolverEngine& solver_engine)
    : d_apply(apply), d_hash(0)
{
  // Compute hash value of function applications based on the current function
  // argument model values.
  for (size_t i = 1, size = apply.num_children(); i < size; ++i)
  {
    d_values.emplace_back(solver_engine.value(apply[i]));
    d_hash += std::hash<Node>{}(d_values.back());
  }
  // Cache value of function application
  d_value = solver_engine.value(apply);
}

const Node&
FunSolver::Apply::get() const
{
  return d_apply;
}

const Node&
FunSolver::Apply::value() const
{
  return d_value;
}

bool
FunSolver::Apply::operator==(const Apply& other) const
{
  assert(d_values.size() == other.d_values.size());
  for (size_t i = 0, size = d_values.size(); i < size; ++i)
  {
    if (d_values[i] != other.d_values[i])
    {
      return false;
    }
  }
  return true;
}

size_t
FunSolver::Apply::hash() const
{
  return d_hash;
}

}  // namespace bzla::fun
