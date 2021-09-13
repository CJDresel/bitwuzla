#ifndef BZLALS__GMP_RANDSTATE_H
#define BZLALS__GMP_RANDSTATE_H

#include <gmpxx.h>

#include <cstdint>

namespace bzla {

/**
 * A GMP gmp_randstate_t wrapper.
 * We use this to avoid having to include gmp headers in header files.
 */
struct GMPRandState
{
  /** Constructor. */
  GMPRandState(uint32_t seed)
  {
    gmp_randinit_mt(d_gmp_randstate);
    gmp_randseed_ui(d_gmp_randstate, seed);
  }
  /** Destructor. */
  ~GMPRandState() { gmp_randclear(d_gmp_randstate); }

  /** The GMP integer value. */
  gmp_randstate_t d_gmp_randstate;
};

}  // namespace bzla

#endif
