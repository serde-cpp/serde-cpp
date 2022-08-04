#pragma once

#include <memory>
#include <string>
#include <serde/error.h>
#include <serde/result.hpp>
#include <serde/de/deserializer.h>

///////////////////////////////////////////////////////////////////////////////
// Serde YAML detail
///////////////////////////////////////////////////////////////////////////////
namespace serde_yaml::detail {

auto DeserializerNew(std::string&& str) -> std::unique_ptr<serde::Deserializer>;
auto DeserializerParse(serde::Deserializer* de) -> cpp::result<void, serde::Error>;

} // namespace serde_yaml::detail

