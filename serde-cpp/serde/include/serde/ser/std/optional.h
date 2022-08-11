#pragma once

#include <optional>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::optional> {
  template<typename T>
  static void serialize(Serializer& ser, const std::optional<T>& opt) {
    if (opt)
      ser.serialize(*opt);
    else
      ser.serialize_none();
  }
};

} // namespace serde
