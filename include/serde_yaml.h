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

struct Token {
  enum class Kind {
    Seq,
    Map,
    Entry,
  };
  Kind kind;
  size_t count = 0;
};


struct YamlSerializer : public serde::Serializer {

  void handle_prefix() {
    if (!seq.empty()) {
      Token& token = seq.top();
      if (token.kind == Token::Kind::Seq || token.kind == Token::Kind::Map) {
        if (++token.count > 1)
          ss << ", ";
      }
    }
  }

  void serialize_int(int v) final {
    handle_prefix();
    ss << v;
  }

  void serialize_cstr(const char* v) final {
    handle_prefix();
    ss << v;
  }

  void serialize_seq_beg() final {
    handle_prefix();
    ss << '[';
    seq.push({Token::Kind::Seq});
  }

  void serialize_seq_end() final {
    ss << ']';
    seq.pop();
  }

  void serialize_map_beg() final {
    handle_prefix();
    ss << '{';
    seq.push({Token::Kind::Map});
  }
  void serialize_map_end() final {
    ss << '}';
    seq.pop();
  }

  void serialize_map_key_beg() final {
    handle_prefix();
    seq.push({Token::Kind::Entry});
  }
  void serialize_map_key_end() final {
    ss << ": ";
  }

  void serialize_map_value_beg() final {
  }
  void serialize_map_value_end() final {
    seq.pop();
  }


  YamlSerializer() = default;
  virtual ~YamlSerializer() = default;

  std::string str() { return ss.str(); }

 private:
  std::stringstream ss;
  std::stack<Token> seq;
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
  serde::serialize(ser, obj);
  return Ok(ser.str());
}

} // namespace serde_yaml
