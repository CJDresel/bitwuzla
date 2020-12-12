#ifndef BZLALS__BITVECTOR_H
#define BZLALS__BITVECTOR_H

#include <cstdint>
#include <memory>
#include <string>

namespace bzlals {

struct GMPMpz;
class RNG;

class BitVector
{
 public:
  /** Create a true bit-vector (value 1 of size 1). */
  static BitVector mk_true();
  /** Create a false bit-vector (value 0 of size 1). */
  static BitVector mk_false();
  /** Create a zero bit-vector of given size. */
  static BitVector mk_zero(uint32_t size);
  /** Create a one bit-vector of given size. */
  static BitVector mk_one(uint32_t size);
  /** Create a ones bit-vector of given size. */
  static BitVector mk_ones(uint32_t size);
  /** Create a minimum signed value bit-vector of given size. */
  static BitVector mk_min_signed(uint32_t size);
  /** Create a maximum signed value bit-vector of given size. */
  static BitVector mk_max_signed(uint32_t size);
  /**
   * Create a if-then-else over the given bit-vectors.
   * c: The condition.
   * t: The then branch.
   * e: The else branch.
   */
  static BitVector bvite(const BitVector& c,
                         const BitVector& t,
                         const BitVector& e);

  /** Default constructor. */
  BitVector();
  /** Construct a zero bit-vector of given size. */
  BitVector(uint32_t size);
  /** Construct a random bit-vector of given size. */
  BitVector(uint32_t size, const RNG& rng);
  /**
   * Construct a random bit-vector of given size with the given range.
   * size     : The size of the bit-vector.
   * rng      : Random number generator.
   * from     : Lower bound of given range (inclusive).
   * to       : Upper bound of given range (inclusive).
   * is_signed: True to interpret the given range as signed, else unsigned.
   */
  BitVector(uint32_t size,
            const RNG& rng,
            const BitVector& from,
            const BitVector& to,
            bool is_signed = false);

  /**
   * Construct a new bit-vector of given size and randomly set bits within given
   * index range. Bits outside of given index range are initialized with zero.
   */
  BitVector(uint32_t size, const RNG& rng, uint32_t idx_hi, uint32_t idx_lo);

  /**
   * Construct a bit-vector of given size from given binary string.
   * size : The bit-vector size, must be >= the length of 'value'.
   * value: A binary string representing the value of the bit-vector. If the
   *        length of this string is > 'size', the value is zero extended.
   */
  BitVector(uint32_t size, const std::string& value);
  /**
   * Construct a bit-vector of given size from given uint64 value.
   * size : The bit-vector size.
   * value: A uint64 representing the bit-vector value, if the value can not be
   *        represented with 'size' bits, it is truncated.
   */
  BitVector(uint32_t size, uint64_t value);
  /** Copy constructor. */
  BitVector(const BitVector& other);

  /** Destructor. */
  ~BitVector();

  /** Copy assignment operator. */
  BitVector& operator=(const BitVector& other);

  /** Set the value of this bit-vector to the given unsigned (in place). */
  void iset(uint64_t value);
  /** Set the value of this bit-vector to the value of 'other' (in place). */
  void iset(const BitVector& other);
  /**
   * Set the value of this bit-vector to a random value between 'from' and 'to'
   * (in place).
   */
  void iset(const RNG& rng,
            const BitVector& from,
            const BitVector& to,
            bool is_signed);

  /** Equality comparison operator. */
  bool operator==(const BitVector& other);
  /** Disequality comparison operator. */
  bool operator!=(const BitVector& other);

  /** Return a string representation of this bit-vector. */
  std::string to_string() const;
  /**
   * Return a uint64_t representation of this bit-vector.
   * Size of this bit-vector must be <= 64.
   */
  uint64_t to_uint64() const;

  /** Return the size of this bit-vector. */
  uint32_t get_size() const { return d_size; }

  /**
   * Compare this bit-vector with given bit-vector.
   * Return 0 if 'this' and 'other' are equal, -1 if 'this' is unsigned less
   * than 'other', and 1 if 'this' is unsigned greater than 'other'.
   */
  int32_t compare(const BitVector& other) const;
  /**
   * Compare this bit-vector with given bit-vector.
   * Return 0 if 'this' and 'other' are equal, -1 if 'this' is signed less
   * than 'other', and 1 if 'this' is signed greater than 'other'.
   */
  int32_t signed_compare(const BitVector& other) const;

  /** Return true if the bit at given index is 1, and false otherwise. */
  bool get_bit(uint32_t idx) const;
  /** Set the bit at given index to the given value. */
  void set_bit(uint32_t idx, bool value);
  /** Flip the bit at given index (in place). */
  void flip_bit(uint32_t idx);
  /** Return true if the lsb (index 0) is 1, and false otherwise. */
  bool get_lsb() const;
  /** Return true if the msb (index size - 1) is 1, and false otherwise. */
  bool get_msb() const;

  /** Return true if this bit-vector is one and of size 1. */
  bool is_true() const;
  /** Return true if this bit-vector is zero and of size 1. */
  bool is_false() const;
  /** Return true if this bit-vector is zero. */
  bool is_zero() const;
  /** Return true if this bit-vector is ones (a bit-vector of all 1 bits). */
  bool is_ones() const;
  /** Return true if this bit-vector is one. */
  bool is_one() const;
  /** Return true if this bit-vector is the minimum signed value. */
  bool is_min_signed() const;
  /** Return true if this bit-vector is the maximum signed value. */
  bool is_max_signed() const;

  /**
   * Return true if the addition of this and the given bit-vector produces an
   * overflow.
   */
  bool is_uadd_overflow(const BitVector& other) const;
  /**
   * Return true if a the multiplication of this and the given bit-vector
   * produces an overflow.
   */
  bool is_umul_overflow(const BitVector& other) const;

  /** Get number of trailing zeros (counted from lsb). */
  uint32_t count_trailing_zeros() const;
  /** Get number of leading zeros (counted from msb). */
  uint32_t count_leading_zeros() const;
  /** Get number of leading ones (counted from msb). */
  uint32_t count_leading_ones() const;

  /** Bit-vector operations. ------------------------------------------------ */

  /** Negation. */
  BitVector bvneg() const;
  /** Bit-wise not. */
  BitVector bvnot() const;
  /** Increment. */
  BitVector bvinc() const;
  /** Decrement. */
  BitVector bvdec() const;
  /** And reduction. Returns true bit-vector if all bits are 1, else false. */
  BitVector bvredand() const;
  /** Or reduction. Returns true bit-vector if one bit is 1, else false. */
  BitVector bvredor() const;

  /** Addition. */
  BitVector bvadd(const BitVector& other) const;
  /** Subtraction. */
  BitVector bvsub(const BitVector& other) const;
  /** Bit-wise and. */
  BitVector bvand(const BitVector& other) const;
  /** Implication. */
  BitVector bvimplies(const BitVector& other) const;
  /** Bit-wise nand. */
  BitVector bvnand(const BitVector& other) const;
  /** Bit-wise nor. */
  BitVector bvnor(const BitVector& other) const;
  /** Bit-wise or. */
  BitVector bvor(const BitVector& other) const;
  /** Bit-wise xnor. */
  BitVector bvxnor(const BitVector& other) const;
  /** Bit-wise xor. */
  BitVector bvxor(const BitVector& other) const;
  /** Equality. */
  BitVector bveq(const BitVector& other) const;
  /** Disequality. */
  BitVector bvne(const BitVector& other) const;
  /** Unsigned less than. */
  BitVector bvult(const BitVector& other) const;
  /** Unsigned less than or equal. */
  BitVector bvule(const BitVector& other) const;
  /** Unsigned greater than. */
  BitVector bvugt(const BitVector& other) const;
  /** Unsigned greater than or equal. */
  BitVector bvuge(const BitVector& other) const;
  /** Signed less than. */
  BitVector bvslt(const BitVector& other) const;
  /** Signed less than or equal. */
  BitVector bvsle(const BitVector& other) const;
  /** Signed greater than. */
  BitVector bvsgt(const BitVector& other) const;
  /** Signed greater than or equal. */
  BitVector bvsge(const BitVector& other) const;
  /** Logical left shift. Shift value is given as an unsigned integer. */
  BitVector bvshl(uint32_t shift) const;
  /** Logical left shift. Shift value is given as a bit-vector. */
  BitVector bvshl(const BitVector& other) const;
  /** Logical right shift. Shift value is given as an unsigned integer. */
  BitVector bvshr(uint32_t shift) const;
  /** Logical right shift. Shift value is given as a bit-vector. */
  BitVector bvshr(const BitVector& other) const;
  /** Arithmetic right shift. */
  BitVector bvashr(const BitVector& other) const;
  /** Multiplication. */
  BitVector bvmul(const BitVector& other) const;
  /** Unsigned division. */
  BitVector bvudiv(const BitVector& other) const;
  /** Unsigned remainder. */
  BitVector bvurem(const BitVector& other) const;
  /** Signed division. */
  BitVector bvsdiv(const BitVector& other) const;
  /** Signed remainder. */
  BitVector bvsrem(const BitVector& other) const;

  /**
   * Concatenation.
   * Given bit-vector is concatenated (at the right, the lsb side) to this
   * bit-vector.
   */
  BitVector bvconcat(const BitVector& other) const;

  /**
   * Extract a bit range from this bit-vector.
   * idx_hi: The upper bit-index of the range (inclusive).
   * idx_lo: The lower bit-index of the range (inclusive).
   */
  BitVector bvextract(uint32_t idx_hi, uint32_t idx_lo) const;

  /**
   * Zero extension.
   * n: The number of bits to extend this bit-vector with.
   */
  BitVector bvzext(uint32_t n) const;
  /**
   * Sign extension.
   * n: The number of bits to extend this bit-vector with.
   */
  BitVector bvsext(uint32_t n) const;

  /**
   * Calculate modular inverse for this bit-vector by means of the Extended
   * Euclidian Algorithm.
   *
   * Note: Bit-vector must be odd. The greatest common divisor gcd (c, 2^bw)
   *       must be (and is, in this case) always 1.
   */
  BitVector bvmodinv() const;

  /** In place versions of bit-vector operations. --------------------------- */

  /** Negation (in place). */
  void ibvneg(const BitVector& bv) const;
  /** Bit-wise not (in placeconst BitVector& bv. */
  void ibvnot(const BitVector& bv) const;
  /** Increment (in placeconst BitVector& bv. */
  void ibvinc(const BitVector& bv) const;
  /** Decrement (in placeconst BitVector& bv. */
  void ibvdec(const BitVector& bv) const;
  /**
   * And reduction (in place).
   * Returns true bit-vector if all bits are 1, else false.
   */
  void ibvredand(const BitVector& bv) const;
  /**
   * Or reduction (in place).
   * Returns true bit-vector if one bit is 1, else false.
   */
  void ibvredor(const BitVector& bv) const;

  /** Addition (in place). */
  void ibvadd(const BitVector& bv0, const BitVector& bv1) const;
  /** Subtraction (in place). */
  void ibvsub(const BitVector& bv0, const BitVector& bv1) const;
  /** Bit-wise and (in place). */
  void ibvand(const BitVector& bv0, const BitVector& bv1) const;
  /** Implication (in place). */
  void ibvimplies(const BitVector& bv0, const BitVector& bv1) const;
  /** Bit-wise nand (in place). */
  void ibvnand(const BitVector& bv0, const BitVector& bv1) const;
  /** Bit-wise nor (in place). */
  void ibvnor(const BitVector& bv0, const BitVector& bv1) const;
  /** Bit-wise or (in place). */
  void ibvor(const BitVector& bv0, const BitVector& bv1) const;
  /** Bit-wise xnor (in place). */
  void ibvxnor(const BitVector& bv0, const BitVector& bv1) const;
  /** Bit-wise xor (in place). */
  void ibvxor(const BitVector& bv0, const BitVector& bv1) const;
  /** Equality (in place). */
  void ibveq(const BitVector& bv0, const BitVector& bv1) const;
  /** Disequality (in place). */
  void ibvne(const BitVector& bv0, const BitVector& bv1) const;
  /** Unsigned less than (in place). */
  void ibvult(const BitVector& bv0, const BitVector& bv1) const;
  /** Unsigned less than or equal (in place). */
  void ibvule(const BitVector& bv0, const BitVector& bv1) const;
  /** Unsigned greater than (in place). */
  void ibvugt(const BitVector& bv0, const BitVector& bv1) const;
  /** Unsigned greater than or equal (in place). */
  void ibvuge(const BitVector& bv0, const BitVector& bv1) const;
  /** Signed less than (in place). */
  void ibvslt(const BitVector& bv0, const BitVector& bv1) const;
  /** Signed less than or equal (in place). */
  void ibvsle(const BitVector& bv0, const BitVector& bv1) const;
  /** Signed greater than (in place). */
  void ibvsgt(const BitVector& bv0, const BitVector& bv1) const;
  /** Signed greater than or equal (in place). */
  void ibvsge(const BitVector& bv0, const BitVector& bv1) const;
  /**
   * Logical left shift (in place).
   * Shift value is given as an unsigned integer.
   */
  void ibvshl(const BitVector& bv1, uint32_t shift) const;
  /**
   * Logical left shift (in place).
   * Shift value is given as a bit-vector.
   */
  void ibvshl(const BitVector& bv0, const BitVector& bv1) const;
  /**
   * Logical right shift (in place).
   * Shift value is given as an unsigned integer.
   */
  void ibvshr(const BitVector& bv1, uint32_t shift) const;
  /**
   * Logical right shift (in place).
   * Shift value is given as a bit-vector.
   */
  void ibvshr(const BitVector& bv0, const BitVector& bv1) const;
  /** Arithmetic right shift (in place). */
  void ibvashr(const BitVector& bv0, const BitVector& bv1) const;
  /** Multiplication (in place). */
  void ibvmul(const BitVector& bv0, const BitVector& bv1) const;
  /** Unsigned division (in place). */
  void ibvudiv(const BitVector& bv0, const BitVector& bv1) const;
  /** Unsigned remainder (in place). */
  void ibvurem(const BitVector& bv0, const BitVector& bv1) const;
  /** Signed division (in place). */
  void ibvsdiv(const BitVector& bv0, const BitVector& bv1) const;
  /** Signed remainder (in place). */
  void ibvsrem(const BitVector& bv0, const BitVector& bv1) const;

  /**
   * Concatenation (in place).
   * Bit-vector 'bv1' is concatenated (at the right, the lsb side) to 'bv0'.
   */
  void ibvconcat(const BitVector& bv0, const BitVector& bv1) const;

  /**
   * Extract a bit range from bit-vector 'bv' (in place).
   * idx_hi: The upper bit-index of the range (inclusive).
   * idx_lo: The lower bit-index of the range (inclusive).
   */
  void ibvextract(const BitVector& bv, uint32_t idx_hi, uint32_t idx_lo) const;

  /**
   * Zero extension (in place).
   * n: The number of bits to extend bit-vector 'bv' with.
   */
  void ibvzext(const BitVector& bv, uint32_t n) const;
  /**
   * Sign extension (in place).
   * n: The number of bits to extend bit-vector 'bv' with.
   */
  void ibvsext(const BitVector& bv, uint32_t n) const;

  /**
   * Create a if-then-else over the given bit-vectors (in place).
   * c: The condition.
   * t: The then branch.
   * e: The else branch.
   */
  void ibvite(const BitVector& c, const BitVector& t, const BitVector& e);

  /**
   * Calculate modular inverse for this bit-vector by means of the Extended
   * Euclidian Algorithm (in place).
   *
   * Note: Bit-vector 'bv' must be odd. The greatest common divisor
   *       gcd (c, 2^bw) must be (and is, in this case) always 1.
   */
  void ibvmodinv(const BitVector& bv) const;

 private:
  /**
   * Count leading zeros or ones.
   * zeros: True to determine number of leading zeros, false to count number
   *        of leading ones.
   */
  uint32_t count_leading(bool zeros) const;
  /**
   * Get the first limb and return the number of limbs needed to represented
   * given bit-vector if all zero limbs are disregarded.
   */
  uint32_t get_limb(void* limb, uint32_t nbits_rem, bool zeros) const;
  /**
   * Return true if this bit-vector can be represented with a uint64_t.
   * If true, uint64_t representation is stored in 'res'.
   */
  bool shift_is_uint64(uint32_t* res) const;

  /** The size of this bit-vector. */
  uint32_t d_size               = 0;
  /** The bit-vector value. */
  std::unique_ptr<GMPMpz> d_val = nullptr;
};

}  // namespace bzlals

#endif
