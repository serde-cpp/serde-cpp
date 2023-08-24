#pragma once

#include <cxxopts.hpp>

namespace serde_gen::init {

/// Initialization functions called from MAIN function.
/// These functions handle INPUT ARGUMENTS and start SERDE GENERATION.

auto create_option_list() -> cxxopts::Options;
bool handle_help_version(const cxxopts::Options &option_list, const cxxopts::ParseResult &options);
auto validate_options(const cxxopts::ParseResult &options) -> int;
auto run_serde_generator(const cxxopts::ParseResult &options) -> int;

}  // namespace serde_gen::init