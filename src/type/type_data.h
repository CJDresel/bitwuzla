#ifndef BZLA_TYPE_TYPE_DATA_H_INCLUDED
#define BZLA_TYPE_TYPE_DATA_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <variant>
#include <vector>

namespace bzla::type {

class Type;
class TypeManager;

class TypeData
{
  friend class TypeManager;

 public:
  enum class Kind
  {
    BOOL,
    BV,
    FP,
    RM,
    ARRAY,
    FUN
  };

  TypeData() = delete;
  ~TypeData();

  /**
   * @return The type id.
   */
  uint64_t get_id() const;

  /**
   * @return The type kind.
   */
  Kind get_kind() const;

  /**
   * Return the types for function and array types.
   *
   * @return The vector of stored types.
   */
  const std::vector<Type>& get_types() const;

  /**
   * @return The size of a bit-vector type.
   */
  uint64_t get_bv_size() const;

  /**
   * @return The exponent size of a floating-point type.
   */
  uint64_t get_fp_exp_size() const;

  /**
   * @return The significand size of a floating-point type.
   */
  uint64_t get_fp_sig_size() const;

  /** Increase the reference count by one. */
  void inc_ref();

  /**
   * Decrease the reference count by one.
   *
   * If reference count becomes zero, this type data object will be
   * automatically garbage collected.
   */
  void dec_ref();

 private:
  /** Constructor. */
  TypeData(TypeManager* mgr, Kind kind, const std::vector<Type>& types = {});
  /** Constructor for creating bit-vector type data. */
  TypeData(TypeManager* mgr, uint64_t size);
  /** Constructor for creating floating-point type data. */
  TypeData(TypeManager* mgr, uint64_t exp_size, uint64_t sig_size);

  /** Pointer to type manager that owns this object. */
  TypeManager* d_mgr = nullptr;
  /** Type id. */
  uint64_t d_id = 0;
  /** Type kind. */
  Kind d_kind;
  /** Reference count. */
  uint32_t d_refs = 0;

  /**
   * Variant that either stores the
   * (1) size of a bit-vector type (for Kind::BV)
   * (2) exponent and significand size of a floating-point type (for Kind::FP)
   * (3) types for array and function types (for Kind::ARRAY, Kind::FUN).
   */
  std::variant<uint64_t, std::pair<uint64_t, uint64_t>, std::vector<Type>>
      d_data;
};

/**
 * Hash struct used for hash consing type data.
 */
struct TypeDataHash
{
  static constexpr size_t s_primes[4] = {
      333444569u, 76891121u, 456790003u, 111130391u};
  size_t operator()(const TypeData* d) const;
};

/**
 * Comparison struct used for hash consing type data.
 */
struct TypeDataKeyEqual
{
  bool operator()(const TypeData* d0, const TypeData* d1) const;
};

}  // namespace bzla::type
#endif
