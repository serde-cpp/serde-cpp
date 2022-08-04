#pragma once

#include <memory>
#include <string>
#include <serde/error.h>
#include <serde/result.hpp>
#include <serde/ser/serializer.h>

///////////////////////////////////////////////////////////////////////////////
// Serde YAML detail
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml::detail {

auto SerializerNew() -> std::unique_ptr<serde::Serializer>;
auto SerializerOutput(serde::Serializer* ser) -> cpp::result<std::string, serde::Error>;

} // namespace serde_yaml::detail

