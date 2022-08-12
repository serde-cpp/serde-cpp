#include "serde_yaml/ser_yaml.h"

#include <stack>
#include <iostream>

#include <ryml_std.hpp>
#include <ryml.hpp>
#include <c4/format.hpp>

////////////////////////////////////////////////////////////////////////////////
// Serde YAML
////////////////////////////////////////////////////////////////////////////////
namespace serde_yaml {

class YamlSerializer final : public serde::Serializer {
public:
  YamlSerializer() {
    stack.push(tree.rootref());
  }

  //////////////////////////////////////////////////////////////////////////////
  // Serializer interface
  //////////////////////////////////////////////////////////////////////////////

  // Scalars ///////////////////////////////////////////////////////////////////
  void serialize_bool(bool v) final { serialize_scalar(v); }
  void serialize_i8(int8_t v) final { serialize_scalar(v); }
  void serialize_u8(uint8_t v) final { serialize_scalar(v); }
  void serialize_i16(int16_t v) final { serialize_scalar(v); }
  void serialize_u16(uint16_t v) final { serialize_scalar(v); }
  void serialize_i32(int32_t v) final { serialize_scalar(v); }
  void serialize_u32(uint32_t v) final { serialize_scalar(v); }
  void serialize_i64(int64_t v) final { serialize_scalar(v); }
  void serialize_u64(uint64_t v) final { serialize_scalar(v); }
  void serialize_float(float v) final { serialize_scalar(v); }
  void serialize_double(double v) final { serialize_scalar(v); }
  void serialize_char(char v) final { serialize_scalar(v); }
  void serialize_uchar(unsigned char v) final { serialize_scalar(v); }
  void serialize_cstr(const char* v) final { serialize_scalar(v); }
  void serialize_bytes(const void* val, size_t len) final {
    serialize_scalar(ryml::fmt::cbase64(val, len));
  }

  // Optional //////////////////////////////////////////////////////////////////
  void serialize_none() final { serialize_scalar("null"); }

  // Sequence //////////////////////////////////////////////////////////////////
  void serialize_seq_begin() final {
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

  // Map ///////////////////////////////////////////////////////////////////////
  void serialize_map_begin() final {
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

  // Struct ////////////////////////////////////////////////////////////////////
  void serialize_struct_begin() final {
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

  //////////////////////////////////////////////////////////////////////////////
  // Serialization Utils
  //////////////////////////////////////////////////////////////////////////////

  template<typename T>
  void serialize_scalar(T&& val) {
    auto curr = stack.top();
    if (curr.is_seq())
      curr.append_child() << val;
    else if (curr.has_parent() && curr.parent_is_map()) {
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

auto SerializerOutput(serde::Serializer* ser) -> cpp::result<std::string, serde::Error>
{
  auto yamlser = static_cast<YamlSerializer*>(ser);
  return yamlser->emit();
}

} // namespace detail

} // namespace serde_yaml


