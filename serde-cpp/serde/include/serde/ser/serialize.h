#pragma once

#include <cstddef>

namespace serde {

// Serialization for simple types should ONLY specialize function `serialize` below.
// The specialization for simple types can be in a translation unit (.cpp).
template<typename T>
void serialize(class Serializer& ser, const T& val);

// Serialization for template types should ONLY specialize struct Serialize::serialize below.
// The specialization for template types must be available concretely before the Serializer is invoked!
template<template<typename...> typename T>
struct Serialize {
  template<typename... U>
  static void serialize(class Serializer& ser, const T<U...>& val);
};

// Serialization for template types, forwards to struct Serialize::serialize.
template<template<typename...> typename T, typename... U>
inline void serialize(class Serializer& ser, const T<U...>& val) {
  Serialize<T>::template serialize<U...>(ser, val);
}

// Serialization for string literals, builtin implementation!
template<size_t N>
void serialize(class Serializer& ser, const char (&val)[N]);

} // namespace serde

