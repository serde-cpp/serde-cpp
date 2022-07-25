#pragma once

#include <type_traits>

namespace serde::traits {

// SFINAE for detecting whether T has serialize(Serializer&) member function
template<typename T, typename = void>
struct HasMemberSerialize : public std::false_type {};

template<typename T>
struct HasMemberSerialize<T, std::enable_if_t<std::is_invocable_r_v<void, decltype(&T::serialize), const T&, class Serializer&>>>
: public std::true_type {};

} // namespace serde::traits

