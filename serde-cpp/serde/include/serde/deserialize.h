#pragma once

#include "result.h"
#include "error.h"

namespace serde {

// Deserialization for simple types should ONLY specialize function deserialize below
template<typename T>
void deserialize(class Deserializer& de, T& val);

// Deserialization for template types should ONLY specialize struct Deserialize::deserialize below
template<template<typename...> typename T>
struct Deserialize {
  template<typename... U>
  static void deserialize(class Deserializer& de, T<U...>& val);
};

// Deserialization for template types, forwards to struct Deserialize::deserialize
template<template<typename...> typename T, typename... U>
inline void deserialize(class Deserializer& de, T<U...>& val) {
  Deserialize<T>::template deserialize<U...>(de, val);
}

// Deserialization for string literals, builtin implementation!
template<size_t N>
void deserialize(class Deserializer& de, char (&val)[N]);

} // namespace serde

