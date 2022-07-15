#pragma once

#include <string>
#include "serde.h"
#include "serde_yaml_detail.h"

///////////////////////////////////////////////////////////////////////////////
// Serde YAML
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml {

/// YAML Serializer function from T to yaml string
template<typename T>
auto to_string(T&& obj) -> Result<std::string, serde::Error>
{
  auto ser = detail::SerializerNew();
  ser->serialize(obj);
  return detail::SerializerOutput(ser.get());
}

} // namespace serde_yaml

