#include "node/node.h"

#include <array>
#include <cassert>
#include <iterator>

#include "node/node_data.h"
#include "node/node_manager.h"
#include "printer/printer.h"
#include "solver/fp/floating_point.h"
#include "solver/fp/rounding_mode.h"

namespace bzla {

/* --- Node public --------------------------------------------------------- */

Node::~Node()
{
  if (!is_null())
  {
    assert(d_data);
    d_data->dec_ref();
  }
}

Node::Node(const Node& other) : d_data(other.d_data)
{
  if (d_data)
  {
    d_data->inc_ref();
  }
}

Node&
Node::operator=(const Node& other)
{
  if (other.d_data)
  {
    other.d_data->inc_ref();
  }
  if (d_data)
  {
    d_data->dec_ref();
  }
  d_data = other.d_data;
  return *this;
}

Node::Node(Node&& other)
{
  if (d_data)
  {
    d_data->dec_ref();
  }
  d_data       = other.d_data;
  other.d_data = nullptr;
}

Node&
Node::operator=(Node&& other)
{
  if (d_data)
  {
    d_data->dec_ref();
  }
  d_data       = other.d_data;
  other.d_data = nullptr;
  return *this;
}

uint64_t
Node::id() const
{
  if (d_data)
  {
    return d_data->get_id();
  }
  return 0;
}

node::Kind
Node::kind() const
{
  if (is_null())
  {
    return node::Kind::NULL_NODE;
  }
  return d_data->get_kind();
}

const Type&
Node::type() const
{
  assert(!is_null());
  return d_data->get_type();
}

bool
Node::is_null() const
{
  return d_data == nullptr;
}

bool
Node::is_value() const
{
  return d_data->get_kind() == node::Kind::VALUE;
}

bool
Node::is_const() const
{
  return d_data->get_kind() == node::Kind::CONSTANT;
}

size_t
Node::num_children() const
{
  if (is_null())
  {
    return 0;
  }
  return d_data->get_num_children();
}

const Node&
Node::operator[](size_t index) const
{
  assert(d_data != nullptr);
  return d_data->get_child(index);
}

size_t
Node::num_indices() const
{
  assert(!is_null());
  return d_data->get_num_indices();
}

uint64_t
Node::index(size_t index) const
{
  assert(!is_null());
  assert(num_indices() > 0);
  return d_data->get_index(index);
}

template <>
const bool&
Node::value() const
{
  assert(!is_null());
  assert(type().is_bool());
  return d_data->get_value<bool>();
}

template <>
const BitVector&
Node::value() const
{
  assert(!is_null());
  assert(type().is_bv());
  return d_data->get_value<BitVector>();
}

template <>
const RoundingMode&
Node::value() const
{
  assert(!is_null());
  assert(type().is_rm());
  return d_data->get_value<RoundingMode>();
}

template <>
const FloatingPoint&
Node::value() const
{
  assert(!is_null());
  assert(type().is_fp());
  return d_data->get_value<FloatingPoint>();
}

std::optional<std::reference_wrapper<const std::string>>
Node::symbol() const
{
  assert(!is_null());
  return d_data->get_symbol();
}

Node::iterator
Node::begin() const
{
  if (!is_null())
  {
    return d_data->begin();
  }
  return nullptr;
}

Node::iterator
Node::end() const
{
  if (!is_null())
  {
    return d_data->end();
  }
  return nullptr;
}

/* --- Node private -------------------------------------------------------- */

Node::Node(node::NodeData* data) : d_data(data)
{
  assert(data != nullptr);
  d_data->inc_ref();
};

/* --- Other --------------------------------------------------------------- */

bool
operator==(const Node& a, const Node& b)
{
  return a.d_data == b.d_data;
}

bool
operator!=(const Node& a, const Node& b)
{
  return a.d_data != b.d_data;
}

std::ostream&
operator<<(std::ostream& out, const Node& node)
{
  Printer::print(out, node);
  return out;
}

}  // namespace bzla

namespace std {

size_t
hash<bzla::Node>::operator()(const bzla::Node& node) const
{
  return node.id();
}

size_t
hash<bzla::Node*>::operator()(const bzla::Node* node) const
{
  return node->id();
}

}  // namespace std
