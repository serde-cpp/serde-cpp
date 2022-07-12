#pragma once

#include <string>
#include <sstream>
#include <stack>
#include "serde.h"

namespace serde_yaml {

struct Error final {
  enum class Kind {
    Invalid,
  };

  Error(Kind kind, std::string&& text) : kind(kind), text(std::move(text)) {};

  Kind kind;
  std::string text;
};

class YamlSerializer : public serde::Serializer {

  struct Token {
    enum class Kind {
      Seq,
      Map,
      Entry,
    };
    Kind kind;
    size_t count = 0;
  };

  void handle_prefix() {
    if (!stack.empty()) {
      Token& token = stack.top();
      if (token.kind == Token::Kind::Seq || token.kind == Token::Kind::Map) {
        if (++token.count > 1)
          ss << ", ";
      }
    }
  }

  void serialize_bool(bool v) final {
    handle_prefix();
    ss << std::boolalpha << v << std::noboolalpha;
  }

  void serialize_int(int v) final {
    handle_prefix();
    ss << v;
  }

  void serialize_cstr(const char* v) final {
    handle_prefix();
    ss << v;
  }

  void serialize_none() final {
    handle_prefix();
    ss << "null";
  }

  void serialize_seq_begin() final {
    handle_prefix();
    ss << '[';
    stack.push({Token::Kind::Seq});
  }

  void serialize_seq_end() final {
    ss << ']';
    stack.pop();
  }

  void serialize_map_begin() final {
    handle_prefix();
    ss << '{';
    stack.push({Token::Kind::Map});
  }
  void serialize_map_end() final {
    ss << '}';
    stack.pop();
  }

  void serialize_map_key_begin() final {
    handle_prefix();
    stack.push({Token::Kind::Entry});
  }
  void serialize_map_key_end() final {
    ss << ": ";
  }

  void serialize_map_value_begin() final {
  }
  void serialize_map_value_end() final {
    stack.pop();
  }

  void serialize_struct_begin() final {
    handle_prefix();
    ss << '{';
    stack.push({Token::Kind::Map});
  }
  void serialize_struct_end() final {
    ss << '}';
    stack.pop();
  }

  void serialize_struct_field_begin(const char* name) final {
    handle_prefix();
    stack.push({Token::Kind::Entry});
    ss << name << ": ";
  }
  void serialize_struct_field_end() final {
    stack.pop();
  }

public:

  YamlSerializer() = default;
  virtual ~YamlSerializer() = default;

  std::string str() { return ss.str(); }

private:
  std::stringstream ss;
  std::stack<Token> stack;
};

template<typename T>
auto from_str(std::string_view str) -> Result<T, Error>
{
  return Ok(T());
}

template<typename T>
auto to_string(T&& obj) -> Result<std::string, Error>
{
  auto ser = YamlSerializer();
  ser.serialize(obj);
  return Ok(ser.str());
}

} // namespace serde_yaml
