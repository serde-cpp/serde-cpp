#pragma once

#include <cstdint>
#include <stdexcept>
#include "deserialize.h"
#include "deserializer.h"

namespace serde {

namespace detail {
template<typename T>
inline void deserialize_signed_integer(Deserializer& de, T& val) {
  if constexpr (sizeof(std::decay_t<T>) == 1) {
    de.deserialize_i8(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 2) {
    de.deserialize_i16(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 4) {
    de.deserialize_i32(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 8) {
    int64_t v; // long long int workaround
    de.deserialize_i64(v);
    val = v;
  } else {
    static_assert(sizeof(std::decay_t<T>) <= 8, "unsupported signed integer size");
  }
}

template<typename T>
inline void deserialize_unsigned_integer(Deserializer& de, T& val) {
  if constexpr (sizeof(std::decay_t<T>) == 1) {
    de.deserialize_u8(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 2) {
    de.deserialize_u16(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 4) {
    de.deserialize_u32(val);
  }
  else if constexpr (sizeof(std::decay_t<T>) == 8) {
    uint64_t v; // long long unsigned int workaround
    de.deserialize_u64(v);
    val = v;
  } else {
    static_assert(sizeof(std::decay_t<T>) <= 8, "unsupported unsigned integer size");
  }
}
} // namespace detail

template<>
inline void deserialize(Deserializer& de, bool& v)
{
  de.deserialize_bool(v);
}

template<>
inline void deserialize(Deserializer& de, int& v)
{
  detail::deserialize_signed_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, short int& v)
{
  detail::deserialize_signed_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, long int& v)
{
  detail::deserialize_signed_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, long long int& v)
{
  detail::deserialize_signed_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, unsigned int& v)
{
  detail::deserialize_unsigned_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, short unsigned int& v)
{
  detail::deserialize_unsigned_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, long unsigned int& v)
{
  detail::deserialize_unsigned_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, long long unsigned int& v)
{
  detail::deserialize_unsigned_integer(de, v);
}

template<>
inline void deserialize(Deserializer& de, float& v)
{
  de.deserialize_float(v);
}

template<>
inline void deserialize(Deserializer& de, double& v)
{
  de.deserialize_double(v);
}

template<>
inline void deserialize(Deserializer& de, char& v)
{
  de.deserialize_char(v);
}

template<>
inline void deserialize(Deserializer& de, signed char& v)
{
  char c;
  de.deserialize_char(c);
  v = c;
}

template<>
inline void deserialize(Deserializer& de, unsigned char& v)
{
  de.deserialize_uchar(v);
}

template<>
inline void deserialize(Deserializer& de, char* & cstr)
{
  throw std::logic_error("Cannot deserialize to char*, length is unknown. Use char[N] or std::string");
}

template<>
inline void deserialize(Deserializer& de, const char* & cstr)
{
  throw std::logic_error("Cannot deserialize to const char*, it is const and length is unknown. Use char[N] or std::string");
}

template<size_t N>
inline void deserialize(class Deserializer& de, char (&val)[N])
{
  de.deserialize_cstr(val, N);
}

} // namespace serde

