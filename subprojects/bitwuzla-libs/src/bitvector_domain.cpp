#include "bitvector_domain.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "gmpmpz.h"
#include "gmprandstate.h"
#include "rng.h"
#include "wheel_factorizer.h"

namespace bzlals {

/*----------------------------------------------------------------------------*/

BitVectorDomain::BitVectorDomain(uint32_t size)
    : d_lo(BitVector::mk_zero(size)), d_hi(BitVector::mk_ones(size))
{
}

BitVectorDomain::BitVectorDomain(const BitVector &lo, const BitVector &hi)
    : d_lo(lo), d_hi(hi)
{
  assert(lo.size() == hi.size());
  d_has_fixed_bits = !d_lo.is_zero() || !d_hi.is_ones();
}

BitVectorDomain::BitVectorDomain(const std::string &value)
{
  uint32_t size = value.size();
  assert(size > 0);
  std::string lo(value);
  std::string hi(value);
  std::replace(lo.begin(), lo.end(), 'x', '0');
  std::replace(hi.begin(), hi.end(), 'x', '1');
  d_lo = BitVector(size, lo);
  d_hi = BitVector(size, hi);
  d_has_fixed_bits = !d_lo.is_zero() || !d_hi.is_ones();
}

BitVectorDomain::BitVectorDomain(const BitVector &bv) : d_lo(bv), d_hi(bv)
{
  d_has_fixed_bits = true;
}

BitVectorDomain::BitVectorDomain(uint32_t size, uint64_t value)
    : BitVectorDomain(BitVector(size, value))
{
}

BitVectorDomain::BitVectorDomain(const BitVectorDomain &other)
    : d_lo(other.d_lo), d_hi(other.d_hi)
{
  d_has_fixed_bits = !d_lo.is_zero() || !d_hi.is_ones();
}

BitVectorDomain::~BitVectorDomain()
{
}

uint32_t
BitVectorDomain::size() const
{
  assert(d_lo.size() == d_hi.size());
  return d_lo.size();
}

bool
BitVectorDomain::is_valid() const
{
  return d_lo.bvnot().ibvor(d_hi).is_ones();
}

bool
BitVectorDomain::is_fixed() const
{
  return d_lo.compare(d_hi) == 0;
}

bool
BitVectorDomain::has_fixed_bits() const
{
  assert(is_valid());
  return d_has_fixed_bits;
}

bool
BitVectorDomain::is_fixed_bit(uint32_t idx) const
{
  assert(idx < size());
  return d_lo.get_bit(idx) == d_hi.get_bit(idx);
}

bool
BitVectorDomain::is_fixed_bit_true(uint32_t idx) const
{
  assert(idx < size());
  bool b = d_lo.get_bit(idx);
  if (!b) return false;
  return b == d_hi.get_bit(idx);
}

bool
BitVectorDomain::is_fixed_bit_false(uint32_t idx) const
{
  assert(idx < size());
  bool b = d_lo.get_bit(idx);
  if (b) return false;
  return b == d_hi.get_bit(idx);
}

void
BitVectorDomain::fix_bit(uint32_t idx, bool value)
{
  assert(idx < size());
  d_lo.set_bit(idx, value);
  d_hi.set_bit(idx, value);
}

bool
BitVectorDomain::match_fixed_bits(const BitVector &bv) const
{
  return bv.bvand(d_hi).ibvor(d_lo).compare(bv) == 0;
}

BitVectorDomain &
BitVectorDomain::operator=(const BitVectorDomain &other)
{
  if (&other == this) return *this;
  d_lo = other.d_lo;
  d_hi = other.d_hi;
  return *this;
}

bool
BitVectorDomain::operator==(const BitVectorDomain &other) const
{
  return d_lo.compare(other.d_lo) == 0 && d_hi.compare(other.d_hi) == 0;
}

BitVectorDomain
BitVectorDomain::bvnot() const
{
  return BitVectorDomain(d_hi.bvnot(), d_lo.bvnot());
}

BitVectorDomain
BitVectorDomain::bvshl(const BitVector &shift) const
{
  assert(shift.size() == size());
  return BitVectorDomain(d_lo.bvshl(shift), d_hi.bvshl(shift));
}

BitVectorDomain
BitVectorDomain::bvshr(const BitVector &shift) const
{
  assert(shift.size() == size());
  return BitVectorDomain(d_lo.bvshr(shift), d_hi.bvshr(shift));
}

BitVectorDomain
BitVectorDomain::bvashr(const BitVector &shift) const
{
  assert(shift.size() == size());
  return BitVectorDomain(d_lo.bvashr(shift), d_hi.bvashr(shift));
}

BitVectorDomain
BitVectorDomain::bvconcat(const BitVector &bv) const
{
  return BitVectorDomain(d_lo.bvconcat(bv), d_hi.bvconcat(bv));
}

BitVectorDomain
BitVectorDomain::bvextract(uint32_t idx_hi, uint32_t idx_lo) const
{
  assert(idx_hi >= idx_lo);
  return BitVectorDomain(d_lo.bvextract(idx_hi, idx_lo),
                         d_hi.bvextract(idx_hi, idx_lo));
}

BitVector
BitVectorDomain::get_factor(RNG *rng,
                            const BitVector &num,
                            const BitVector &excl_min,
                            uint64_t limit) const
{
  WheelFactorizer wf(num, limit);
  std::vector<BitVector> factors;

  while (true)
  {
    const BitVector *fact = wf.next();
    if (!fact) break;
    factors.emplace_back(*fact);
    if (rng == nullptr) break;
  }

  /* Pick factor from stack. Random (combination) if 'rng' is given. */
  if (!factors.empty())
  {
    uint32_t n_factors = factors.size();
    if (rng)
    {
      /* To determine all possible combinations can be very expensive. We'll
       * try for a limited number of times, and if none matches, we return 0. */
      for (uint32_t cnt = 0; cnt < 1000; ++cnt)
      {
        /* number of factors to combine */
        uint32_t n = rng->pick<uint32_t>(1, n_factors);
        /* Move selected factors to front of the stack and combine.
         * This ensures that we don't pick a factor twice, e.g., 2 2 3 can be
         * combined into { 2, 3, 2*2, 2*3, 2*2*3 }. */
        BitVector mul(num.size());
        for (uint32_t i = 0; i < n; ++i)
        {
          uint32_t j = rng->pick<uint32_t>(i, n_factors - 1);
          if (i != j)
          {
            std::swap(factors[i], factors[j]);
          }
          if (!mul.is_zero())
          {
            assert(!factors[i].is_umul_overflow(mul));
            BitVector tmp = factors[i].bvmul(mul);
            if (tmp.compare(num) > 0)
            {
              continue;
            }
            mul.iset(tmp);
          }
          else
          {
            mul.iset(factors[i]);
          }
        }
        assert(!mul.is_null());
        if (mul.compare(excl_min) > 0 && match_fixed_bits(mul))
        {
          return mul;
        }
      }
    }
    else
    {
      assert(n_factors == 1);
      return factors[0];
    }
  }
  return BitVector();
}

std::string
BitVectorDomain::to_string() const
{
  std::string res(d_lo.to_string());
  std::string hi(d_hi.to_string());
  for (uint32_t i = 0, n = size(); i < n; ++i)
  {
    if (res[i] != hi[i])
    {
      if (res[i] == '0' && hi[i] == '1')
      {
        res[i] = 'x';
      }
      else
      {
        assert(res[i] == '1' && hi[i] == '0');
        res[i] = 'i';
      }
    }
  }
  return res;
}

/*----------------------------------------------------------------------------*/

BitVectorDomainGenerator::BitVectorDomainGenerator(
    const BitVectorDomain &domain)
    : BitVectorDomainGenerator(domain, nullptr, domain.d_lo, domain.d_hi)
{
}

BitVectorDomainGenerator::BitVectorDomainGenerator(
    const BitVectorDomain &domain, const BitVector &min, const BitVector &max)
    : BitVectorDomainGenerator(domain, nullptr, min, max)
{
}

BitVectorDomainGenerator::BitVectorDomainGenerator(
    const BitVectorDomain &domain, RNG *rng)
    : BitVectorDomainGenerator(domain, rng, domain.d_lo, domain.d_hi)
{
}

BitVectorDomainGenerator::BitVectorDomainGenerator(
    const BitVectorDomain &domain,
    RNG *rng,
    const BitVector &min,
    const BitVector &max)
    : d_domain(domain), d_rng(rng)
{
  uint32_t cnt          = 0;
  uint32_t size         = domain.size();
  const BitVector &hi   = d_domain.d_hi;
  const BitVector &lo   = d_domain.d_lo;
  const BitVector &mmin = lo.compare(min) <= 0 ? min : lo;
  const BitVector &mmax = hi.compare(max) >= 0 ? max : hi;

  for (uint32_t i = 0; i < size; ++i)
  {
    if (!d_domain.is_fixed_bit(i)) cnt += 1;
  }

  if (cnt && mmin.compare(hi) <= 0 && mmax.compare(lo) >= 0)
  {
    assert(mmin.compare(lo) >= 0);
    assert(mmax.compare(hi) <= 0);

    /* Set unconstrained bits to the minimum value that corresponds to a
     * generated value >= mmin. */
    d_bits_min.reset(new BitVector(BitVector::mk_zero(cnt)));
    for (uint32_t i = 0, j = 0, j0 = 0; i < size; ++i)
    {
      uint32_t idx_i = size - 1 - i;
      bool bit       = mmin.get_bit(idx_i);
      if (!d_domain.is_fixed_bit(idx_i))
      {
        assert(j < cnt);
        d_bits_min->set_bit(cnt - 1 - j, bit);
        if (!bit) j0 = j;
        j += 1;
      }
      else if (d_domain.is_fixed_bit_true(idx_i) && !bit)
      {
        break;
      }
      else if (d_domain.is_fixed_bit_false(idx_i) && bit)
      {
        assert(j > 0);
        assert(!d_bits_min->get_bit(cnt - j0 - 1));
        d_bits_min->set_bit(cnt - j0 - 1, true);
        for (uint32_t k = j0 + 1; k < cnt; ++k)
        {
          d_bits_min->set_bit(cnt - 1 - k, false);
        }
        break;
      }
    }

    /* Set unconstrained bits to the maxium value that corresponds to a
     * generated value <= mmax. */
    d_bits_max.reset(new BitVector(BitVector::mk_ones(cnt)));
    for (uint32_t i = 0, j = 0, j0 = 0; i < size; ++i)
    {
      uint32_t idx_i = size - 1 - i;
      bool bit       = mmax.get_bit(idx_i);
      if (!d_domain.is_fixed_bit(idx_i))
      {
        assert(j < cnt);
        d_bits_max->set_bit(cnt - 1 - j, bit);
        if (bit) j0 = j;
        j += 1;
      }
      else if (d_domain.is_fixed_bit_true(idx_i) && !bit)
      {
        assert(j > 0);
        assert(d_bits_max->get_bit(cnt - j0 - 1));
        d_bits_max->set_bit(cnt - j0 - 1, false);
        for (uint32_t k = j0 + 1; k < cnt; ++k)
        {
          d_bits_max->set_bit(cnt - 1 - k, true);
        }
        break;
      }
      else if (d_domain.is_fixed_bit_false(idx_i) && bit)
      {
        break;
      }
    }

    /* If bits_min > bits_max, we can't generate any value. */
    if (d_bits_min->compare(*d_bits_max) <= 0)
    {
      d_bits.reset(new BitVector(*d_bits_min));
    }
  }
#ifndef NDEBUG
  d_min = mmin;
  d_max = mmax;
#endif
}

BitVectorDomainGenerator::~BitVectorDomainGenerator() {}

bool
BitVectorDomainGenerator::has_next() const
{
  assert(d_bits == nullptr
         || (d_bits_min && d_bits->compare(*d_bits_min) >= 0));
  return d_bits && d_bits->compare(*d_bits_max) <= 0;
}

bool
BitVectorDomainGenerator::has_random() const
{
  assert(d_bits == nullptr
         || (d_bits_min && d_bits_min->compare(*d_bits_min) >= 0));
  assert(!d_bits_min || d_bits_max);
  return d_bits_min && d_bits_min->compare(*d_bits_max) <= 0;
}

BitVector
BitVectorDomainGenerator::next()
{
  assert(has_next());
  return generate_next(false);
}

BitVector
BitVectorDomainGenerator::random()
{
  assert(has_random());
  return generate_next(true);
}

BitVector
BitVectorDomainGenerator::generate_next(bool random)
{
  assert(random || d_bits);

  uint32_t size = d_domain.size();
  BitVector res(d_domain.d_lo);

  /* Random always resets d_bits to a random value between d_bits_min and
   * d_bits_max. */
  if (random)
  {
    assert(d_rng);
    assert(d_bits_min);
    assert(d_bits_max);
    if (d_bits == nullptr) d_bits.reset(new BitVector(d_bits_min->size()));
    assert(d_bits->size() == d_bits_min->size());
    d_bits->iset(*d_rng, *d_bits_min, *d_bits_max, false);
  }

  for (uint32_t i = 0, j = 0; i < size; ++i)
  {
    if (!d_domain.is_fixed_bit(i))
    {
      res.set_bit(i, d_bits->get_bit(j++));
    }
  }

  /* If bits is ones, we enumerated all values. */
  if (d_bits->compare(*d_bits_max) == 0)
  {
    /* random never terminates and bits start again at bits_min. */

    if (random)
    {
      d_bits->iset(*d_bits_min);
    }
    else
    {
      d_bits.reset(nullptr);
    }
  }
  else
  {
    d_bits->ibvinc();
  }

  assert(d_bits == nullptr || d_bits->compare(*d_bits_min) >= 0);
  assert(d_bits == nullptr || d_bits->compare(*d_bits_max) <= 0);
#ifndef NDEBUG
  assert(res.compare(d_min) >= 0);
  assert(res.compare(d_max) <= 0);
#endif
  return res;
}

BitVectorDomainSignedGenerator::BitVectorDomainSignedGenerator(
    const BitVectorDomain &domain)
    : BitVectorDomainSignedGenerator(domain,
                                     nullptr,
                                     BitVector::mk_min_signed(domain.size()),
                                     BitVector::mk_max_signed(domain.size()))
{
}

BitVectorDomainSignedGenerator::BitVectorDomainSignedGenerator(
    const BitVectorDomain &domain, const BitVector &min, const BitVector &max)
    : BitVectorDomainSignedGenerator(domain, nullptr, min, max)
{
}

BitVectorDomainSignedGenerator::BitVectorDomainSignedGenerator(
    const BitVectorDomain &domain, RNG *rng)
    : BitVectorDomainSignedGenerator(domain,
                                     rng,
                                     BitVector::mk_min_signed(domain.size()),
                                     BitVector::mk_max_signed(domain.size()))
{
}

BitVectorDomainSignedGenerator::BitVectorDomainSignedGenerator(
    const BitVectorDomain &domain,
    RNG *rng,
    const BitVector &min,
    const BitVector &max)
    : d_rng(rng)
{
  uint32_t size          = domain.size();
  BitVector zero         = BitVector::mk_zero(size);
  BitVector ones         = BitVector::mk_ones(size);
  int32_t min_scomp_zero = min.signed_compare(zero);
  int32_t max_scomp_zero = max.signed_compare(zero);
  if (min_scomp_zero < 0)
  {
    d_gen_lo.reset(new BitVectorDomainGenerator(
        domain, rng, min, max_scomp_zero < 0 ? max : ones));
    d_gen_cur = d_gen_lo.get();
  }
  if (max_scomp_zero >= 0)
  {
    d_gen_hi.reset(new BitVectorDomainGenerator(
        domain, rng, min_scomp_zero >= 0 ? min : zero, max));
    if (d_gen_cur == nullptr) d_gen_cur = d_gen_hi.get();
  }
}

BitVectorDomainSignedGenerator::~BitVectorDomainSignedGenerator() {}

bool
BitVectorDomainSignedGenerator::has_next()
{
  if (d_gen_cur == nullptr) return false;
  if (!d_gen_cur->has_next())
  {
    if (d_gen_cur == d_gen_lo.get() && d_gen_hi)
    {
      d_gen_cur = d_gen_hi.get();
      return d_gen_cur->has_next();
    }
    return false;
  }
  return true;
}

bool
BitVectorDomainSignedGenerator::has_random()
{
  if (d_gen_cur == nullptr) return false;
  if (!d_gen_cur->has_random())
  {
    if (d_gen_cur == d_gen_lo.get() && d_gen_hi)
    {
      d_gen_cur = d_gen_hi.get();
      return d_gen_cur->has_random();
    }
    return false;
  }
  return true;
}

BitVector
BitVectorDomainSignedGenerator::next()
{
  assert(has_next());
  return d_gen_cur->next();
}

BitVector
BitVectorDomainSignedGenerator::random()
{
  bool has_random_lo = d_gen_lo ? d_gen_lo->has_random() : false;
  bool has_random_hi = d_gen_hi ? d_gen_hi->has_random() : false;
  if (has_random_lo && has_random_hi)
  {
    return d_rng->flip_coin() ? d_gen_lo->random() : d_gen_hi->random();
  }
  if (has_random_lo)
  {
    return d_gen_lo->random();
  }
  assert(has_random_hi);
  return d_gen_hi->random();
}

std::ostream &
operator<<(std::ostream &out, const BitVectorDomain &d)
{
  out << d.to_string();
  return out;
}
}  // namespace bzlals
