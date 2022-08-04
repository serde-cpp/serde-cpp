#include "serde_yaml/de_yaml.h"

#include <stack>
#include <cstring>
#include <iostream>

#include <ryml_std.hpp>
#include <ryml.hpp>
#include <c4/format.hpp>

////////////////////////////////////////////////////////////////////////////////
// Serde YAML
////////////////////////////////////////////////////////////////////////////////
namespace serde_yaml {

class YamlDeserializer final : public serde::Deserializer {
  std::string yaml;
  ryml::Tree tree;
  std::stack<ryml::NodeRef> stack;
  bool expect_key = false;
  bool entry_find = false;

public:
  YamlDeserializer(std::string yaml) : yaml(std::move(yaml)) {
  }

  void parse() {
    ryml::parse_in_place(ryml::substr(yaml.data(), yaml.length()), &tree);
    stack.push(tree.rootref());
  }

  template<typename T>
  void deserialize_scalar(T& val) {
    auto& curr = stack.top();
    if (!curr.valid() || curr.is_seed() || !curr.get()) {
      std::cerr << "no scalar to extract" << std::endl;
      return; // TODO: mark error
    }

    if (expect_key) {
      if (curr.has_key()) {
        from_chars(curr.key(), &val); // TODO: check return
        //std::cout << "got key " << val << std::endl;
      }
      else {
        std::cerr << "no key to extract" << std::endl;
      }
    }
    else if (curr.has_val()) {
      from_chars(curr.val(), &val); // TODO: check return
      //std::cout << "got val " << val << std::endl;
      if (curr.has_parent() && curr.parent_is_seq()) {
        //std::cout << "next_sibling" << std::endl;
        curr = curr.next_sibling();
      }
    }
    else {
      std::cerr << "no value to extract" << std::endl;
    }
  }

  void deserialize_bool(bool& val) final { deserialize_scalar(val); }
  void deserialize_i8(int8_t& val) final { deserialize_scalar(val); }
  void deserialize_u8(uint8_t& val) final { deserialize_scalar(val); }
  void deserialize_i16(int16_t& val) final { deserialize_scalar(val); }
  void deserialize_u16(uint16_t& val) final { deserialize_scalar(val); }
  void deserialize_i32(int32_t& val) final { deserialize_scalar(val); }
  void deserialize_u32(uint32_t& val) final { deserialize_scalar(val); }
  void deserialize_i64(int64_t& val) final { deserialize_scalar(val); }
  void deserialize_u64(uint64_t& val) final { deserialize_scalar(val); }
  void deserialize_float(float& val) final { deserialize_scalar(val); }
  void deserialize_double(double& val) final { deserialize_scalar(val); }
  void deserialize_char(char& val) final { deserialize_scalar(val); }
  void deserialize_uchar(unsigned char& val) final { deserialize_scalar(val); }

  void deserialize_cstr(char* val, size_t len) final {
    auto& curr = stack.top();
    if (!curr.valid() || curr.is_seed() || !curr.get()) {
      std::cerr << "no scalar to extract" << std::endl;
      return; // TODO: mark error
    }

    if (expect_key) {
      if (curr.has_key()) {
        auto cstr = curr.key();
        len = std::min(cstr.len + 1, len);
        if (len) {
          std::strncpy(val, cstr.data(), len);
          val[len-1] = '\0';
        }
        //std::cout << "got key " << val << std::endl;
      }
      else {
        std::cerr << "no key to extract" << std::endl;
      }
    }
    else if (curr.has_val()) {
      auto cstr = curr.val();
      len = std::min(cstr.len + 1, len);
      if (len) {
        std::strncpy(val, cstr.data(), len);
        val[len-1] = '\0';
      }
      //std::cout << "got val " << val << std::endl;
      if (curr.has_parent() && curr.parent_is_seq()) {
        //std::cout << "next_sibling" << std::endl;
        curr = curr.next_sibling();
      }
    }
    else {
      //std::cerr << "no value to extract" << std::endl;
      if (len)
        *val = '\0';
    }
  }
  void deserialize_bytes(unsigned char* val, size_t len) final {
  }

  void deserialize_length(size_t& len) final {
    auto& curr = stack.top();
    if (!curr.valid() || curr.is_seed() || !curr.get()) {
      std::cerr << "not valid node to check length" << std::endl;
      return; // TODO: mark error
    }
    if (expect_key) {
      if (curr.has_key()) {
        //std::cout << "got key " << val << std::endl;
        len = curr.key().len;
      }
      else {
        std::cerr << "no key to check length" << std::endl;
      }
    }
    else if (curr.has_val()) {
      len = curr.val().len;
    }
    else {
      //std::cerr << "no value to check length" << std::endl;
      len = 0;
    }
  }

  // Optional //////////////////////////////////////////////////////////////////
  void deserialize_is_some(bool& val) final {
    auto& curr = stack.top();
    if (!curr.valid() || curr.is_seed() || !curr.get()) {
      std::cerr << "not valid node to check for some" << std::endl;
      return; // TODO: mark error
    }
    if (expect_key) {
      if (curr.has_key()) {
        //std::cout << "got key " << val << std::endl;
        val = !curr.key_is_null();
      }
      else {
        std::cerr << "no key to check for some" << std::endl;
      }
    }
    else if (curr.has_val()) {
      val = !curr.val_is_null();
    }
    else {
      std::cerr << "no value to check for some" << std::endl;
    }
  }

  void deserialize_none() final {
    auto& curr = stack.top();
    if (!curr.valid() || curr.is_seed() || !curr.get()) {
      std::cerr << "not valid node to extract none" << std::endl;
      return; // TODO: mark error
    }
    if (expect_key) {
      if (curr.has_key()) {
        //std::cout << "got key " << val << std::endl;
      }
      else {
        std::cerr << "no key to extract none" << std::endl;
      }
    }
    else if (curr.has_val()) {
      //std::cout << "got val " << val << std::endl;
      if (curr.has_parent() && curr.parent_is_seq()) {
        //std::cout << "next_sibling" << std::endl;
        curr = curr.next_sibling();
      }
    }
    else {
      std::cerr << "no value to extract none" << std::endl;
    }
  }


  void deserialize_seq_begin() final {
    auto curr = stack.top();
    if (!curr.is_seq()) {
      std::cerr << "no sequence to begin" << std::endl;
      return;
    }
    //std::cout << "num_children "  << curr.num_children() << std::endl;
    stack.push(curr.first_child());
  }

  void deserialize_seq_end() final {
    //auto curr = stack.top();
    //if (!curr.parent_is_seq()) {
      //std::cerr << "no sequence to end" << std::endl;
      //return;
    //}
    stack.pop();
  }

  void deserialize_seq_size(size_t& val) final {
    auto curr = stack.top();
    if (!curr.is_seq()) {
      std::cerr << "no sequence to count" << std::endl;
      return;
    }
    val = curr.num_children();
  }

  void deserialize_map_begin() final {
    auto curr = stack.top();
    if (!curr.is_map()) {
      std::cerr << "no map to begin" << std::endl;
      return;
    }
    //std::cout << "num_children "  << curr.num_children() << std::endl;
    stack.push(curr.first_child());
  }

  void deserialize_map_size(size_t& val) final {
    auto curr = stack.top();
    if (!curr.is_map()) {
      std::cerr << "no map to count" << std::endl;
      return;
    }
    val = curr.num_children();
  }

  void deserialize_map_end() final {
    stack.pop();
  }

  void deserialize_map_key_begin() final {
    // TODO: check for has_key
    //std::cout << "expect key" << std::endl;
    expect_key = true;
  }

  void deserialize_map_key_end() final {
    // TODO: check for was true
    //std::cout << "unexpect key" << std::endl;
    expect_key = false;
  }

  void deserialize_map_key_find(const char* key) final {
    auto curr = stack.top();
    if (!curr.has_parent() || !curr.parent_is_map()) {
      std::cerr << "no map to find key" << std::endl;
      return;
    }
    auto child = curr.find_sibling({key, std::strlen(key)});
    if (!child.valid() || child.is_seed() || !child.get()) {
      std::cerr << "key not found in map" << std::endl;
      return;
    }
    stack.push(child);
    entry_find = true;
  }

  void deserialize_map_value_begin() final {
  }
  void deserialize_map_value_end() final {
    auto& curr = stack.top();
    if (entry_find) {
      stack.pop();
    } else {
      curr = curr.next_sibling();
    }
  }

  // Struct ////////////////////////////////////////////////////////////////////
  void deserialize_struct_begin() final {
    deserialize_map_begin();
  }

  void deserialize_struct_end() final {
    deserialize_map_end();
  }

  void deserialize_struct_field_begin(const char* name) final {
    deserialize_map_key_find(name);
    deserialize_map_value_begin();
  }

  void deserialize_struct_field_end() final {
    deserialize_map_value_end();
  }

};


namespace detail {

auto DeserializerNew(std::string&& str) -> std::unique_ptr<serde::Deserializer>
{
  return std::make_unique<YamlDeserializer>(std::move(str));
}

auto DeserializerParse(serde::Deserializer* de) -> cpp::result<void, serde::Error>
{
  auto yamlde = static_cast<YamlDeserializer*>(de);
  yamlde->parse();
  return {};
}

} // namespace detail

} // namespace serde_yaml
