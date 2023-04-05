#ifndef BZLA_SOLVER_FP_FP_SOLVER_H_INCLUDED
#define BZLA_SOLVER_FP_FP_SOLVER_H_INCLUDED

#include "backtrack/object.h"
#include "backtrack/vector.h"
#include "solver/fp/word_blaster.h"
#include "solver/solver.h"

namespace bzla::fp {

class FpSolver : public Solver
{
 public:
  /**
   * Determine if given term is a leaf node for other solvers than the
   * floating-point solver.
   * @param term The term to query.
   */
  static bool is_theory_leaf(const Node& term);

  FpSolver(Env& env, SolverState& state);
  ~FpSolver();

  bool check() override;

  Node value(const Node& term) override;

  void register_term(const Node& term) override;

 private:
  /** Compute assignment for given leaf term. */
  Node assignment(const Node& term);

  /** The word blaster. */
  WordBlaster d_word_blaster;
  /** The current queue of nodes to word-blast on the next check() call. */
  backtrack::vector<Node> d_word_blast_queue;
  /** Index in d_word_blast_queue to mark already word-blasted terms. */
  backtrack::object<size_t> d_word_blast_index;
};

}  // namespace bzla::fp

#endif
