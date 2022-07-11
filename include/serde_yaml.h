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


struct YamlSerializer : public serde::Serializer {
  void serialize_int(int v) final {
    if (!seq.empty() && ++seq.top() > 1)
      ss << ", ";
    ss << v;
  }

  void serialize_seq_beg() final {
    if (!seq.empty() && ++seq.top() > 1)
      ss << ", ";
    ss << '[';
    seq.push(0);
  }

  void serialize_seq_end() final {
    ss << ']';
    seq.pop();
  }


  YamlSerializer() = default;
  virtual ~YamlSerializer() = default;

  std::string str() { return ss.str(); }

 private:
  std::stringstream ss;
  std::stack<size_t> seq;
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
  serialize(ser, obj);
  return Ok(ser.str());
}

} // namespace serde_yaml
