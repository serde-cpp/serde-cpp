#include "serde_yaml/de_yaml.h"

#include <stack>
#include <cstring>
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
      if (curr.parent_is_seq()) {
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
  void deserialize_float(float&) final { }
  void deserialize_double(double&) final { }
  void deserialize_char(char& val) final { deserialize_scalar(val); }
  void deserialize_uchar(unsigned char& val) final { deserialize_scalar(val); }

  void deserialize_cstr(char* val, size_t len) final {
    auto& curr = stack.top();
    if (!curr.is_val() || !curr.valid() || curr.is_seed()) {
      std::cerr << "no value to extract" << std::endl;
      return; // TODO: mark error
    }
    auto cstr = curr.val();
    len = std::min(cstr.len, len);
    if (len) std::strncpy(val, cstr.data(), len);
    //if (curr.parent_is_seq()) {
      ////std::cout << "next_sibling" << std::endl;
      //curr = curr.next_sibling();
    //}
  }
  void deserialize_bytes(unsigned char* val, size_t len) final {
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

  void deserialize_seq_count(size_t& val) final {
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
  void deserialize_map_value_begin() final {
  }
  void deserialize_map_value_end() final {
    auto& curr = stack.top();
    curr = curr.next_sibling();
  }

};


namespace detail {

auto DeserializerNew(std::string&& str) -> std::unique_ptr<serde::Deserializer>
{
  return std::make_unique<YamlDeserializer>(std::move(str));
}

auto DeserializerParse(serde::Deserializer* de) -> Result<void, serde::Error>
{
  auto yamlde = static_cast<YamlDeserializer*>(de);
  yamlde->parse();
  return Ok();
}

} // namespace detail

} // namespace serde_yaml
