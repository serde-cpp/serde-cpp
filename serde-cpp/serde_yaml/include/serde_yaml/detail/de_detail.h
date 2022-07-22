#pragma once

#include <memory>
#include <string>
#include "serde/de/deserializer.h"

///////////////////////////////////////////////////////////////////////////////
// Serde YAML detail
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml::detail {

auto DeserializerNew(std::string&& str) -> std::unique_ptr<serde::Deserializer>;
auto DeserializerParse(serde::Deserializer* de) -> Result<void, serde::Error>;

} // namespace serde_yaml::detail


