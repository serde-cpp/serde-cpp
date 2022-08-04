#pragma once

#include <string>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::basic_string> {
  template<typename CharT, typename... U>
  static void serialize(Serializer& ser, const std::basic_string<CharT, U...>& str) {
    static_assert(std::is_same_v<CharT, char>, "serialize only supports char-based std::string");
    ser.serialize_cstr(str.data());
  }
};

} // namespace serde

