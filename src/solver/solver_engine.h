#ifndef BZLA_SOLVER_SOLVER_ENGINE_H_INCLUDED
#define BZLA_SOLVER_SOLVER_ENGINE_H_INCLUDED

#include "backtrack/assertion_stack.h"
#include "backtrack/backtrackable.h"
#include "backtrack/pop_callback.h"
#include "backtrack/unordered_set.h"
#include "node/node.h"
#include "rewrite/rewriter.h"
#include "solver/bv/bv_solver.h"
#include "solver/fp/fp_solver.h"
#include "solver/fun/fun_solver.h"
#include "solver/result.h"

namespace bzla {

class SolvingContext;

class SolverEngine
{
  friend SolvingContext;

 public:
  SolverEngine(SolvingContext& context);

  /** Get value of given term. Queries corresponding solver for value. */
  Node value(const Node& term);

  /** Add a lemma. */
  void lemma(const Node& lemma);

  /** @return Rewriter. */
  Rewriter& rewriter();

  /** @return Options. */
  const option::Options& options() const;

  /** @return Solver engine backtrack manager. */
  backtrack::BacktrackManager* backtrack_mgr();

 private:
  // temporary helpers, should be moved to corresponding solvers as static
  // method
  static bool is_array_leaf(const Node& term);
  static bool is_quant_leaf(const Node& term);

  /**
   * Solve current set of assertions.
   *
   * @note Should only be called by SolvingContext, hence the friend
   *       declaration.
   */
  Result solve();

  /** Synchronize d_backtrack_mgr up to given level. */
  void sync_scope(size_t level);

  /**
   * Process current set of assertions via process_assertion().
   */
  void process_assertions();

  /**
   * Processes given assertion and distributes reachable theory leafs to
   * solvers.
   */
  void process_assertion(const Node& assertion, size_t level);

  /** Process lemmas added via lemma(). */
  void process_lemmas();

  /** Associated solving context. */
  SolvingContext& d_context;

  /** Solver engine backtrack manager. */
  backtrack::BacktrackManager d_backtrack_mgr;
  /** Callback to sync with solving context backtrack manager on pop(). */
  backtrack::PopCallback d_pop_callback;
  /** Assertion view of unprocessed assertions. */
  backtrack::AssertionView& d_assertions;
  /** Cache used by process_assertion(). */
  backtrack::unordered_set<Node> d_register_cache;

  /** Lemmas added via lemma(). */
  std::vector<Node> d_lemmas;
  /** Lemma cache. */
  std::unordered_set<Node> d_lemma_cache;

  /** Result of latest solve() call. */
  Result d_sat_state;

  /** Theory solvers. */
  bv::BvSolver d_bv_solver;
  fp::FpSolver d_fp_solver;
  fun::FunSolver d_fun_solver;
  // array::ArraySolver d_array_solver;
  // quant::QuantSolver d_quant_solver;
};

}  // namespace bzla

#endif
