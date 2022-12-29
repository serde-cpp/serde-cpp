#pragma once

#include <string>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::basic_string> {
  template<typename CharT, typename Traits, typename Alloc>
  static void deserialize(Deserializer& de, std::basic_string<CharT, Traits, Alloc>& str) {
    static_assert(std::is_same_v<CharT, char>, "deserialize only supports char-based std::string");
    size_t len = 0;
    de.deserialize_length(len);
    str.resize(len);
    de.deserialize_cstr(str.data(), len + 1);
  }
};

} // namespace serde

