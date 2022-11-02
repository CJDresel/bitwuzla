#ifndef BZLA_SOLVER_BV_BV_SOLVER_H_INCLUDED
#define BZLA_SOLVER_BV_BV_SOLVER_H_INCLUDED

#include <unordered_map>

#include "backtrack/assertion_stack.h"
#include "backtrack/vector.h"
#include "bitblast/aig/aig_cnf.h"
#include "bitblast/aig_bitblaster.h"
#include "sat/sat_solver.h"
#include "solver/solver.h"

namespace bzla::bv {

class BvSolver : public Solver
{
 public:
  BvSolver(SolvingContext& context);
  ~BvSolver();

  Result check() override;

  Node value(const Node& term) override;

 private:
  /** Sat interface used for d_cnf_encoder. */
  class BitblastSatSolver;

  /** Recursively bit-blast `term`. */
  void bitblast(const Node& term);

  void register_abstraction(const Node& term);

  /** Return encoded bits associated with bit-blasted term. */
  const bb::AigBitblaster::Bits& get_bits(const Node& term) const;
  /** Determine if `term` is a leaf node for the bit-vector solver. */
  bool is_leaf(const Node& term) const;

  /** Query assignment from SAT solver. */
  Node get_assignment(const Node& term) const;

  /** The current set of assertions. */
  backtrack::AssertionView& d_assertion_view;
  backtrack::vector<Node> d_assumptions;

  /** AIG Bit-blaster. */
  bb::AigBitblaster d_bitblaster;
  /** Cached to store bit-blasted terms and their encoded bits. */
  std::unordered_map<Node, bb::AigBitblaster::Bits> d_bitblaster_cache;
  /** CNF encoder for AIGs. */
  std::unique_ptr<bb::AigCnfEncoder> d_cnf_encoder;
  /** SAT solver used for solving bit-blasted formula. */
  std::unique_ptr<sat::SatSolver> d_sat_solver;
  /** SAT solver interface for CNF encoder, which wraps `d_sat_solver`. */
  std::unique_ptr<BitblastSatSolver> d_bitblast_sat_solver;
  /** Result of the last check() call. */
  Result d_sat_state = Result::UNKNOWN;
};

}  // namespace bzla::bv

#endif
