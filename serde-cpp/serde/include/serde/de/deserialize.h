#pragma once

#include <cstddef>

namespace serde {

// forward-declaration
class Deserializer;


// Deserialization for simple types should ONLY specialize function `deserialize` below.
// The specialization for simple types can be in a translation unit (.cpp).
template<typename T>
void deserialize(Deserializer& de, T& val);

// Deserialization for simple types but with the perks of specialization only on instantiation.
// deserialize is not defined here because we check for specialization in constexpr evaluation.
template<typename T, typename = void>
struct DeserializeT;
// {
//   static void deserialize(Deserializer& de, const T& val);
// };


// Deserialization for template types should ONLY specialize struct Deserialize::deserialize below.
// The specialization for template types must be available concretely before the Deserializer is invoked!
template<template<typename...> typename T>
struct Deserialize {
  template<typename... U>
  static void deserialize(Deserializer& de, T<U...>& val);
};

// Deserialization for template types, forwards to struct Deserialize::deserialize.
template<template<typename...> typename T, typename... U>
inline void deserialize(Deserializer& de, T<U...>& val) {
  Deserialize<T>::template deserialize<U...>(de, val);
}


// Deserialization for template types with only integral parameter, e.g. std::bitset.
template<template<auto...> typename T>
struct DeserializeN {
  template<auto... N>
  static void deserialize(Deserializer& de, T<N...>& val);
};

// Deserialization for template types with only integral parameter, e.g. std::bitset
// forwards to struct DeserializeN::deserialize.
template<template<auto...> typename T, auto... N>
inline void deserialize(Deserializer& de, T<N...>& val) {
  DeserializeN<T>::template deserialize<N...>(de, val);
}


// Deserialization for template types with typename and integral parameter, e.g. std::array.
template<template<typename, auto, auto...> typename T>
struct DeserializeTN {
  template<typename U, auto... N>
  static void deserialize(Deserializer& de, T<U, N...>& val);
};

// Deserialization for template types with typename and integral parameter, e.g. std::array.
// forwards to struct DeserializeTN::deserialize.
template<template<typename, auto, auto...> typename T, typename U, auto N, auto... M>
inline void deserialize(Deserializer& de, T<U, N, M...>& val) {
  DeserializeTN<T>::template deserialize<U, N, M...>(de, val);
}


// Deserialization for string literals, builtin implementation!
template<size_t N>
void deserialize(Deserializer& de, char (&val)[N]);

} // namespace serde

