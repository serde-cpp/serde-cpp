#pragma once

#include <memory>
#include <string>
#include "serde.h"

namespace serde_yaml {

namespace detail {
auto SerializerNew() -> std::unique_ptr<serde::Serializer>;
auto SerializerOutput(serde::Serializer* ser) -> Result<std::string, serde::Error>;
}

/// YAML Serializer function from T to yaml string
template<typename T>
auto to_string(T&& obj) -> Result<std::string, serde::Error>
{
  auto ser = detail::SerializerNew();
  ser->serialize(obj);
  return detail::SerializerOutput(ser.get());
}

} // namespace serde_yaml

