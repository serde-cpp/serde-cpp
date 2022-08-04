#pragma once

#include <string>
#include <serde/de.h>
#include <serde/error.h>
#include <serde/result.hpp>
#include "detail/de_detail.h"

///////////////////////////////////////////////////////////////////////////////
// Serde YAML
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml {

/// YAML Deserializer function from yaml string to T
template<typename T>
auto from_str(std::string&& str) -> cpp::result<T, serde::Error>
{
  auto de = detail::DeserializerNew(std::move(str));
  std::ignore = detail::DeserializerParse(de.get());
  T obj{};
  de->deserialize(obj);
  return std::move(obj);
}

} // namespace serde_yaml

