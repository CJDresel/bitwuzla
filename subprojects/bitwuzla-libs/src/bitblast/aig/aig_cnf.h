#include "bitblast/aig/aig_manager.h"

namespace bzla::bb {

class SatInterface
{
 public:
  /**
   * Add literal to current clause.
   *
   * @param id Id of literal to be added. 0 terminates the clause.
   */
  virtual void add(int64_t lit) = 0;
  /**
   * Add a set of literals as clause.
   *
   * @param literals List of literals to be added (without terminating 0).
   */
  virtual void add_clause(const std::initializer_list<int64_t>& literals) = 0;

  virtual bool value(int64_t lit) = 0;
};

class AigCnfEncoder
{
 public:
  AigCnfEncoder(SatInterface& sat_solver) : d_sat_solver(sat_solver){};

  /**
   * Recursively encodes AIG node to CNF.
   *
   * @param node The AIG node to encode.
   * @param top_level Indicates whether given node is at the top level, which
   *        enables certain optimization.
   * */
  void encode(const AigNode& node, bool top_level = false);

  int32_t value(const AigNode& node);

 private:
  /** Encode AIG to CNF. */
  void _encode(const AigNode& node);
  /** Ensure that `d_aig_encoded` is big enough to store `aig`. */
  void resize(const AigNode& aig);
  /** Checks whether `aig` was already encoded. */
  bool is_encoded(const AigNode& aig) const;
  /** Mark `aig` as encoded. */
  void set_encoded(const AigNode& aig);

  /** Maps AIG id to flag that indicates whether the AIG was already encoded. */
  std::vector<bool> d_aig_encoded;
  /** SAT solver. */
  SatInterface& d_sat_solver;
};
}  // namespace bzla::bb