#pragma once

#include <string>
#include <sstream>
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

struct YamlSer;

struct YamlSerializer : public serde::Serializer<YamlSerializer, void, Error> {
  auto serialize_bool(bool v) -> Result;
  auto serialize_int(int v) -> Result;
  auto serialize_uint(unsigned int v) -> Result;
  auto serialize_lint(long int v) -> Result;
  auto serialize_llint(long long int v) -> Result;
  auto serialize_luint(unsigned long int v) -> Result;
  auto serialize_lluint(unsigned long long int v) -> Result;
  auto serialize_char(char v) -> Result;
  auto serialize_cstr(char* v) -> Result;
  auto serialize_bytes(unsigned char* v, size_t len) -> Result;
  template<typename T>
  auto serialize_some(T&& v) -> Result;
  auto serialize_none() -> Result;
  auto serialize_tuple() -> Result;
  auto serialize_seq(Fn&& fn) -> Result;
  auto serialize_map(Fn&& fn) -> Result;

  YamlSerializer() = default;
  virtual ~YamlSerializer() = default;

  std::string str() { return ss.str(); }

 private:
  std::stringstream ss;
};

inline auto YamlSerializer::serialize_int(int v) -> Result
{
  ss << v;
  return ::Ok();
}

template<typename T>
auto from_str(std::string_view str) -> Result<T, Error>
{
  return Ok(T());
}

template<typename T>
auto to_string(T&& obj) -> Result<std::string, Error>
{
  auto ser = YamlSerializer();
  auto result = serialize(obj, ser);
  if (result.isErr()) return Err(result.unwrapErr());
  return Ok(ser.str());
}

} // namespace serde_yaml
