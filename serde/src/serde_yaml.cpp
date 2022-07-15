#include <stack>

#include <ryml_std.hpp>
#include <ryml.hpp>
#include <c4/format.hpp>

#include "serde_yaml.h"

namespace serde_yaml {

class YamlSerializer final : public serde::Serializer {
public:
  YamlSerializer() {
    stack.push(tree.rootref());
  }

  // Scalar
  template<typename T>
  void serialize_scalar(T&& val) {
    auto curr = stack.top();
    if (curr.is_seq())
      curr.append_child() << val;
    else if (curr.parent_is_map()) {
      curr.is_seed();
      if (!curr.has_key())
        curr << ryml::key(val);
      else if (!curr.has_val())
        curr << val;
      else {
        curr.append_sibling() << ryml::key(val);
        stack.push(curr.next_sibling());
      }
    }
    else {
      curr << val;
    }
  }

  void serialize_bool(bool v) final {
    serialize_scalar(v);
  }

  void serialize_int(int v) final {
    serialize_scalar(v);
  }

  void serialize_cstr(const char* v) final {
    serialize_scalar(v);
  }

  void serialize_none() final {
  }

  // Sequence
  void serialize_seq_begin(serde::Style style = serde::Style::Fold) final {
    auto curr = stack.top();
    if (curr.is_seq() || curr.is_map()) {
      curr.append_child() |= ryml::SEQ;
      stack.push(curr.last_child());
    }
    else {
      curr |= ryml::SEQ;
    }
  }

  void serialize_seq_end() final {
    auto curr = stack.top();
    if (curr.is_seq())
      stack.pop();
  }

  // Map
  void serialize_map_begin(serde::Style style = serde::Style::Fold) final {
    auto curr = stack.top();
    if (curr.is_seq() || curr.is_map()) {
      curr.append_child() |= ryml::MAP;
      stack.push(curr.last_child());
    }
    else {
      curr |= ryml::MAP;
    }
  }

  void serialize_map_end() final {
    auto curr = stack.top();
    if (curr.is_map())
      stack.pop();
  }

  void serialize_map_key_begin() final {
    auto curr = stack.top();
    curr.append_child();
    stack.push(curr.last_child());
  }

  void serialize_map_key_end() final {
  }

  void serialize_map_value_begin() final {
  }

  void serialize_map_value_end() final {
    stack.pop();
  }

  // Struct
  void serialize_struct_begin(serde::Style style = serde::Style::Fold) final {
    serialize_map_begin();
  }
  void serialize_struct_end() final {
    serialize_map_end();
  }

  void serialize_struct_field_begin(const char* name) final {
    serialize_map_key_begin();
    serialize(name);
    serialize_map_key_end();
    serialize_map_value_begin();
  }
  void serialize_struct_field_end() final {
    serialize_map_value_end();
  }

  std::string emit() const {
    return ryml::emitrs<std::string>(tree);
  }

private:
  ryml::Tree tree;
  std::stack<ryml::NodeRef> stack;
};


namespace detail {

auto SerializerNew() -> std::unique_ptr<serde::Serializer>
{
  return std::make_unique<YamlSerializer>();
}

auto SerializerOutput(serde::Serializer* ser) -> Result<std::string, serde::Error>
{
  auto yamlser = static_cast<YamlSerializer*>(ser);
  return Ok(yamlser->emit());
}

} // namespace detail

} // namespace serde_yaml


