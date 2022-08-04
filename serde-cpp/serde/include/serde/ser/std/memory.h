#pragma once

#include <memory>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::unique_ptr> {
  template<typename T, typename Deleter>
  static void serialize(Serializer& ser, const std::unique_ptr<T, Deleter>& val) {
    if (val)
      ser.serialize(*val);
    else
      ser.serialize_none();
  }
};

template<>
struct Serialize<std::shared_ptr> {
  template<typename T>
  static void serialize(Serializer& ser, const std::shared_ptr<T>& val) {
    if (val)
      ser.serialize(*val);
    else
      ser.serialize_none();
  }
};

} // namespace serde

