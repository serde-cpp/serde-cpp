#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <stack>
#include "serde.h"
#include <assert.h>

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
    serde::Style style;
    size_t indent = 0;
    size_t count = 0;
  };

  void handle_prefix(bool complex = false, serde::Style style = serde::Style::Inline) {
    if (!stack.empty()) {
      Token& token = stack.top();

      switch (token.kind) {
        case Token::Kind::Seq: {
          if (token.style == serde::Style::Inline) {
            assert(!(complex && style == serde::Style::Fold));
            if (++token.count > 1)
              ss << ", ";
          } else if (token.style == serde::Style::Fold) {
            if (++token.count > 1)
              ss << '\n' << std::setw(token.indent + 2);
            ss << "- ";
          }
          break;
        }

        case Token::Kind::Map: {
          if (token.style == serde::Style::Inline) {
            assert(!(complex && style == serde::Style::Fold));
            if (++token.count > 1)
              ss << ", ";
          } else if (token.style == serde::Style::Fold) {
            if (++token.count > 1)
              ss << '\n' << std::setw(token.indent) << "";
          }
          break;
        }

        case Token::Kind::Entry: {
          if (token.style == serde::Style::Fold) {
            if (++token.count > 1 && complex)
              ss << '\n' << std::setw(token.indent) << "";
          }
        }
      }
    }
  }

  void stack_push(Token::Kind kind, serde::Style style) {
    size_t indent = 0;
    if (!stack.empty()) {
      auto& top = stack.top();
      indent = top.indent;
      if (top.kind != Token::Kind::Entry)
        indent += 2;
    }
    stack.push({kind, style, indent, 0});
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

  void serialize_seq_begin(serde::Style style = serde::Style::Fold) final {
    handle_prefix(true, style);
    if (style == serde::Style::Inline)
      ss << '[';
    stack_push(Token::Kind::Seq, style);
  }

  void serialize_seq_end() final {
    if (!stack.empty()) {
      auto& top = stack.top();
      if (top.kind != Token::Kind::Seq)
        return;
      if (top.style == serde::Style::Inline)
        ss << ']';
      else if (top.style == serde::Style::Fold && stack.size() == 1)
        ss << '\n';
      stack.pop();
    }
  }

  void serialize_map_begin(serde::Style style = serde::Style::Fold) final {
    handle_prefix(true, style);
    if (style == serde::Style::Inline)
      ss << '{';
    stack_push(Token::Kind::Map, style);
  }

  void serialize_map_end() final {
    if (!stack.empty()) {
      auto& top = stack.top();
      if (top.kind != Token::Kind::Map)
        return;
      if (top.style == serde::Style::Inline)
        ss << '}';
      else if (top.style == serde::Style::Fold && stack.size() == 1)
        ss << '\n';
      stack.pop();
    }
  }

  void serialize_map_key_begin() final {
    if (!stack.empty()) {
      auto& top = stack.top();
      if (top.kind != Token::Kind::Map)
        return;
      handle_prefix();
      stack_push(Token::Kind::Entry, top.style);
    }
  }
  void serialize_map_key_end() final {
    ss << ": ";
  }

  void serialize_map_value_begin() final {
  }
  void serialize_map_value_end() final {
    stack.pop();
  }

  void serialize_struct_begin(serde::Style style = serde::Style::Fold) final {
    serialize_map_begin(style);
  }
  void serialize_struct_end() final {
    serialize_map_end();
  }

  void serialize_struct_field_begin(const char* name) final {
    serialize_map_key_begin();
    serialize(name);
    serialize_map_key_end();
  }
  void serialize_struct_field_end() final {
    stack.pop();
  }

public:

  YamlSerializer() {
    ss << "---\n";
  }
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
