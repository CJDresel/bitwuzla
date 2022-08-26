#ifndef BZLA_NODE_NODE_MANAGER_H_INCLUDED
#define BZLA_NODE_NODE_MANAGER_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "node/node.h"
#include "node/node_data.h"
#include "type/type_manager.h"

namespace bzla::node {

class NodeManager
{
  friend class NodeData;

 public:
  /* --- Node interface ---------------------------------------------------- */

  /** Create constant of type `t`. */
  Node mk_const(const type::Type& t, const std::string& symbol = "");

  /** Create variable of type `t`. */
  Node mk_var(const type::Type& t, const std::string& symbol = "");

  /** Create value `value` of type `t`. */
  // TODO: Instantiations for bv, fp, rm
  template <class T>
  Node mk_value(const type::Type& t, const T value);

  /** Create node of kind `kind` with given children and indices. */
  Node mk_node(Kind kind,
               const std::vector<Node>& children,
               const std::vector<uint64_t>& indices = {});

  /* --- Type interface ---------------------------------------------------- */

  /** Create boolean type. */
  type::Type mk_bool_type();

  /** Create bit-vector type of size `size`. */
  type::Type mk_bv_type(uint64_t size);

  /**
   * Create floating-point type with exponent size `exp_size` and significand
   * size `sig_size`.
   */
  type::Type mk_fp_type(uint64_t exp_size, uint64_t sig_size);

  /** Create rounding mode type. */
  type::Type mk_rm_type();

  /** Create array type with index type `index` and element type `elem`. */
  type::Type mk_array_type(const type::Type& index, const type::Type& elem);

  /**
   * Create function type with codomain `types[:-1]` and domain `types[-1]`.
   *
   * Note: Last type in `types` corresponds to function domain type.
   */
  type::Type mk_fun_type(const std::vector<type::Type>& types);

  /** Compute type for given node. */
  type::Type compute_type(Kind kind,
                          const std::vector<Node>& children,
                          const std::vector<uint64_t>& indices = {});

  /** Type checking of children and indices based on kind. */
  std::pair<bool, std::string> check_type(
      Kind kind,
      const std::vector<Node>& children,
      const std::vector<uint64_t>& indices = {});

 private:
  void init_id(NodeData* d);

  NodeData* new_data(Kind kind,
                     const std::vector<Node>& children,
                     const std::vector<uint64_t>& indices);

  NodeData* find_or_create_node(Kind kind,
                                const std::vector<Node>& children,
                                const std::vector<uint64_t>& indices);

  void garbage_collect(NodeData* d);

  type::TypeManager d_tm;
  uint64_t d_node_id_counter = 1;
  bool d_in_gc_mode          = false;
  std::vector<std::unique_ptr<NodeData>> d_node_data;
  std::unordered_set<NodeData*, NodeDataHash, NodeDataKeyEqual> d_unique_nodes;
};

}  // namespace bzla::node
#endif
