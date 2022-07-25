#pragma once

#include <optional>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::optional> {
  template<typename U>
  static void serialize(Serializer& ser, const std::optional<U>& opt) {
    if (opt)
      ser.serialize(*opt);
    else
      ser.serialize_none();
  }
};

} // namespace serde

