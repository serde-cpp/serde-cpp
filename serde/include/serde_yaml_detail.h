#pragma once

#include <memory>
#include <string>
#include "serde.h"

///////////////////////////////////////////////////////////////////////////////
// Serde YAML detail
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml::detail {

auto SerializerNew() -> std::unique_ptr<serde::Serializer>;
auto SerializerOutput(serde::Serializer* ser) -> Result<std::string, serde::Error>;

} // namespace serde_yaml::detail

