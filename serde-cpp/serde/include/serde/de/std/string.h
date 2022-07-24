#pragma once

#include <string>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::basic_string> {
  template<typename CharT, typename... U>
  static void deserialize(Deserializer& de, std::basic_string<CharT, U...>& str) {
    static_assert(std::is_same_v<CharT, char>, "deserialize only supports char-based std::string for now");
    size_t len = 0;
    de.deserialize_length(len);
    str.resize(len);
    de.deserialize_cstr(str.data(), len + 1);
  }
};

} // namespace serde

