#ifndef BZLA_SOLVER_BV_BV_SOLVER_H_INCLUDED
#define BZLA_SOLVER_BV_BV_SOLVER_H_INCLUDED

#include "solver/bv/bv_bitblast_solver.h"
#include "solver/solver.h"

namespace bzla::bv {

class BvSolver : public Solver
{
 public:
  /** Determine if `term` is a leaf node for the bit-vector solver. */
  static bool is_leaf(const Node& term);

  /** Construct default value for given Boolean or bit-vector type. */
  static Node default_value(const Type& type);

  BvSolver(SolvingContext& context);
  ~BvSolver();

  Result check() override;

  Node value(const Node& term) override;

 private:
  /** Query leaf assignment from subsolver. */
  Node assignment(const Node& term);

  /** Result of the last check() call. */
  Result d_sat_state = Result::UNKNOWN;

  /** Bitblast subsolver. */
  BvBitblastSolver d_bitblast_solver;
};

}  // namespace bzla::bv

#endif
