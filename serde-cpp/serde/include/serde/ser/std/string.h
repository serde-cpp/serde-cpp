#pragma once

#include <string>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::basic_string> {
  template<typename CharT, typename Traits, typename Alloc>
  static void serialize(Serializer& ser, const std::basic_string<CharT, Traits, Alloc>& str) {
    static_assert(std::is_same_v<CharT, char>, "serialize only supports char-based std::string");
    ser.serialize_cstr(str.data());
  }
};

} // namespace serde
