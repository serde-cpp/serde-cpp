#pragma once

#include <string_view>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::basic_string_view> {
  template<typename CharT, typename... U>
  static void serialize(Serializer& ser, const std::basic_string_view<CharT, U...>& str) {
    static_assert(std::is_same_v<CharT, char>, "serialize only supports char-based std::string_view");
    ser.serialize_cstr(str.c_str());
  }
};

} // namespace serde

