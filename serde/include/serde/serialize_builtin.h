#pragma once

#include "result.h"
#include "serialize.h"
#include "serializer.h"

namespace serde {

// Primitives

template<>
inline void serialize(Serializer& ser, const char* const & cstr)
{
  ser.serialize_cstr(cstr);
}

template<>
inline void serialize(Serializer& ser, const int& v)
{
  ser.serialize_int(v);
}

// Vector
template<template<typename...> typename T, typename... U>
inline void serialize(Serializer& ser, T<U...> const& vec)
{
  ser.serialize_seq_begin();
  for (auto& v : vec) ser.serialize(v);
  ser.serialize_seq_end();
}

} // namespace serde


