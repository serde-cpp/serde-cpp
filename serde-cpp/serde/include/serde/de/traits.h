#pragma once

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////
// Foward-declarations
namespace serde {

class Deserializer;

template<typename T, typename>
struct DeserializeT;

} // namespace serde


////////////////////////////////////////////////////////////////////////////////
// Type Traits
namespace serde::traits {

// Trait for detecting whether T has deserialize(Deserializer&) member function
template<typename T, typename = void>
struct HasMemberDeserialize : public std::false_type {};

template<typename T>
struct HasMemberDeserialize<T, std::enable_if_t<std::is_invocable_r_v<void, decltype(&T::deserialize), T&, Deserializer&>>>
: public std::true_type {};


// Trait for detecting whether T has Deserialize<T, void>::deserialize static function
template<typename T, typename = void>
struct HasDeserialize : public std::false_type {};

template<typename T>
struct HasDeserialize<T, std::enable_if_t<std::is_invocable_r_v<void, decltype(&DeserializeT<T, void>::deserialize), Deserializer&, T&>>>
: public std::true_type {};

} // namespace serde::traits

