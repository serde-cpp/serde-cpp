#pragma once

#include <string>
#include <serde/ser.h>
#include <serde/error.h>
#include <serde/result.h>

#include "detail/ser_detail.h"

///////////////////////////////////////////////////////////////////////////////
// Serde YAML
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml {

/// YAML Serializer function from T to yaml string
template<typename T>
auto to_string(T&& obj) -> Result<std::string, serde::Error>
{
  auto ser = detail::SerializerNew();
  ser->serialize(std::forward<T>(obj));
  return detail::SerializerOutput(ser.get());
}

} // namespace serde_yaml

