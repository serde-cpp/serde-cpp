#pragma once

#include "result.h"

namespace serde {

template<typename S, typename K, typename E>
struct Serializer {
  using Ok = K;
  using Error = E;
  using Result = ::Result<Ok, Error>;
  using Fn = std::function<Result(Serializer&)>;

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

  virtual ~Serializer() = default;
};

template<typename S, typename K, typename E>
inline auto Serializer<S, K, E>::serialize_int(int v) -> Result
{
  auto s = reinterpret_cast<S*>(this);
  return s->serialize_int(v);
}


template<typename D>
struct Deserializer {
};


} // namespace serde

template<typename S, typename K, typename E, typename T>
auto serialize(T& obj, serde::Serializer<S, K, E>& ser) -> typename S::Result;

//template<typename D, typename T>
//auto deserialize(Deserializer<D>& de) -> typename D::Result;
