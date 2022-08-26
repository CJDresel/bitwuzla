#ifndef BZLA_SAT_SAT_SOLVER_H_INCLUDED
#define BZLA_SAT_SAT_SOLVER_H_INCLUDED

#include <cstdint>
#include <string>

namespace bzla::sat {

class SatSolver
{
 public:
  /** The result of a check satisfiability call. */
  enum class Result
  {
    UNKNOWN,
    SAT,
    UNSAT,
  };

  /**
   * Constructor.
   * @param name The name of the underlying SAT solver.
   */
  SatSolver() {}
  /** Destructor. */
  virtual ~SatSolver(){};

  /**
   * Add valid literal to current clause.
   * @param lit The literal to add, 0 to terminate clause..
   */
  virtual void add(int32_t lit) = 0;
  /**
   * Assume valid (non-zero) literal for next call to 'check_sat'.
   * @param lit The literal to assume.
   */
  virtual void assume(int32_t lit) = 0;
  /**
   * Get value of valid non-zero literal.
   * @param lit The literal to query.
   * @return The value of the literal (-1 = false, 1 = true, 0 = unknown).
   */
  virtual int32_t value(int32_t lit) = 0;
  /**
   * Determine if the valid non-zero literal is in the unsat core.
   * @param lit The literal to query.
   * @return True if the literal is in the core, and false otherwise.
   */
  virtual bool failed(int32_t lit) = 0;
  /**
   * Determine if the given literal is implied by the formula.
   * @return 1 if it is implied, -1 if it is not implied and 0 if unknown.
   */
  virtual int32_t fixed(int32_t lit) = 0;
  /**
   * Check satisfiability of current formula.
   * @return The result of the satisfiability check.
   */
  virtual Result solve() = 0;
  /**
   * Configure a termination callback function.
   * @param fun The callback function, returns a value != 0 if sat solver has
   *            been terminated.
   * @param state The argument to the callback function.
   */
  virtual void set_terminate(int32_t (*fun)(void *), void *state) = 0;

  // virtual int32_t repr(int32_t) = 0;

  /**
   * Get the name of this sat solver.
   * @return The name of this sat solver.
   */
  virtual const char *get_name() const = 0;
};

}  // namespace bzla::sat
#endif
