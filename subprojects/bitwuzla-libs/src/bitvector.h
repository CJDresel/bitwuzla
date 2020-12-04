#ifndef BZLALS__BITVECTOR_H
#define BZLALS__BITVECTOR_H

#include <cstdint>
#include <memory>
#include <string>

namespace bzlals {

struct BzlalsMPZ;

// class RNG;

class BitVector
{
 public:
  static BitVector mk_zero(uint32_t size);
  static BitVector mk_one(uint32_t size);
  static BitVector mk_ones(uint32_t size);
  static BitVector mk_min_signed(uint32_t size);
  static BitVector mk_max_signed(uint32_t size);
  static BitVector bvite(const BitVector& c,
                         const BitVector& t,
                         const BitVector& e);

  BitVector(uint32_t size);
  // BitVector(uint32_t size, RNG& rng);
  // BitVector(uint32_t size,
  //          RNG& rng,
  //          const BitVector& from,
  //          const BitVector& to,
  //          bool is_signed = false);
  // BitVector(uint32_t size, RNG& rng, uint32_t idx_hi, uint32_t idx_lo);
  BitVector(uint32_t size, const std::string& bin_str);
  BitVector(uint32_t size, uint64_t value);
  // should this deep copy by default? or do we need an extra copy for this?
  BitVector(BitVector& other);

  ~BitVector();

  std::string to_string() const;

  uint32_t get_size() const { return d_size; }

  int32_t compare(const BitVector& other) const;
  int32_t signed_compare(const BitVector& other) const;

  bool get_bit(uint32_t idx) const;
  void set_bit(uint32_t idx, bool value);
  void flip_bit(uint32_t idx);

  bool is_true() const;
  bool is_false() const;
  bool is_zero() const;
  bool is_ones() const;
  bool is_one() const;
  bool is_min_signed() const;
  bool is_max_signed() const;

  bool is_uadd_overflow(const BitVector& other) const;
  bool is_umul_overflow(const BitVector& other) const;

  uint32_t count_trailing_zeros() const;
  uint32_t count_leading_zeros() const;
  uint32_t count_leading_ones() const;

  BitVector bvneg() const;
  BitVector bvnot() const;
  BitVector bvinc() const;
  BitVector bvdec() const;
  BitVector bvredand() const;
  BitVector bvredor() const;

  BitVector bvadd(const BitVector& other) const;
  BitVector bvsub(const BitVector& other) const;
  BitVector bvand(const BitVector& other) const;
  BitVector bvimplies(const BitVector& other) const;
  BitVector bvnand(const BitVector& other) const;
  BitVector bvnor(const BitVector& other) const;
  BitVector bvor(const BitVector& other) const;
  BitVector bvxnor(const BitVector& other) const;
  BitVector bvxor(const BitVector& other) const;
  BitVector bveq(const BitVector& other) const;
  BitVector bvne(const BitVector& other) const;
  BitVector bvult(const BitVector& other) const;
  BitVector bvule(const BitVector& other) const;
  BitVector bvugt(const BitVector& other) const;
  BitVector bvuge(const BitVector& other) const;
  BitVector bvslt(const BitVector& other) const;
  BitVector bvsle(const BitVector& other) const;
  BitVector bvsgt(const BitVector& other) const;
  BitVector bvsge(const BitVector& other) const;
  BitVector bvshl(const BitVector& other) const;
  BitVector bvshr(const BitVector& other) const;
  BitVector bvashr(const BitVector& other) const;
  BitVector bvmul(const BitVector& other) const;
  BitVector bvudiv(const BitVector& other) const;
  BitVector bvurem(const BitVector& other) const;
  BitVector bvsdiv(const BitVector& other) const;
  BitVector bvsrem(const BitVector& other) const;
  BitVector bvconcat(const BitVector& other) const;
  BitVector bvextract(uint32_t idx_hi, uint32_t idx_lo) const;
  BitVector bvzext(uint32_t n) const;
  BitVector bvsext(uint32_t n) const;

 private:
  uint32_t d_size;
  std::unique_ptr<BzlalsMPZ> d_val;
};

}  // namespace bzlals

#endif
