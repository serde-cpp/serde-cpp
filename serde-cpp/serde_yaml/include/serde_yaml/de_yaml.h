#pragma once

#include <string>
#include <serde/error.h>
#include <serde/result.h>
#include <serde/deserializer.h>
#include "de_detail.h"

///////////////////////////////////////////////////////////////////////////////
// Serde YAML
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml {

/// YAML Deserializer function from yaml string to T
template<typename T>
auto from_str(std::string&& str) -> Result<T, serde::Error>
{
  auto de = detail::DeserializerNew(std::move(str));
  detail::DeserializerParse(de.get());
  T obj{};
  de->deserialize(obj);
  return Ok(obj);
}

} // namespace serde_yaml

