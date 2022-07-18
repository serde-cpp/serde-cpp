#pragma once

namespace serde {

template<typename T>
void serialize(class Serializer& ser, const T& v);

//template<template<typename> typename T, typename U>
//void serialize(class Serializer& ser, const T<U>& v);

template<template<typename...> typename T, typename... U>
void serialize(class Serializer& ser, const T<U...>& v);

} // namespace serde

