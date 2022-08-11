#pragma once

#include <string_view>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::basic_string_view> {
  template<typename CharT, typename Traits>
  static void serialize(Serializer& ser, const std::basic_string_view<CharT, Traits>& str) {
    static_assert(std::is_same_v<CharT, char>, "serialize only supports char-based std::string_view");
    ser.serialize_cstr(str.data());
  }
};

} // namespace serde
