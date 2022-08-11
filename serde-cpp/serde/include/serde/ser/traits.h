#pragma once

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
// Foward-declarations
namespace serde {

class Serializer;

template<typename T, typename>
struct Serialize;

} // namespace serde


////////////////////////////////////////////////////////////////////////////////
// Type Traits
namespace serde::traits {

// Trait for detecting whether T has serialize(Serializer&) member function
template<typename T, typename = void>
struct HasMemberSerialize : public std::false_type {};

template<typename T>
struct HasMemberSerialize<T, std::enable_if_t<std::is_invocable_r_v<void, decltype(&T::serialize), const T&, Serializer&>>>
: public std::true_type {};


// Trait for detecting whether T has Serialize<T, void>::serialize static function
template<typename T, typename = void>
struct HasSerialize : public std::false_type {};

template<typename T>
struct HasSerialize<T, std::enable_if_t<std::is_invocable_r_v<void, decltype(&Serialize<T, void>::serialize), Serializer&, const T&>>>
: public std::true_type {};

} // namespace serde::traits
