#pragma once

#include <type_traits>

namespace serde::traits {

// SFINAE for detecting whether T has deserialize(Deserializer&) member function
template<typename T, typename = void>
struct HasMemberDeserialize : public std::false_type {};

template<typename T>
struct HasMemberDeserialize<T, std::enable_if_t<std::is_invocable_r_v<void, decltype(&T::deserialize), T&, class Deserializer&>>>
: public std::true_type {};

} // namespace serde::traits

