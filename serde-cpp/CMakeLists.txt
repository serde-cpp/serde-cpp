cmake_minimum_required(VERSION 3.5)
project(serde LANGUAGES CXX VERSION 0.2)

#########################################################################################
# Configuration
#########################################################################################
# Default build type to Debug, for release use `cmake -DCMAKE_BUILD_TYPE=Release ..`
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type default to Debug" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Turn on when having trouble building
set(CMAKE_VERBOSE_MAKEFILE OFF)
# Compilation Database for C++ language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default compilation settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -pg -fsanitize=undefined -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -Wall")

#########################################################################################
# Sub-directories
#########################################################################################
add_subdirectory(serde)
add_subdirectory(serde_gen)
add_subdirectory(serde_yaml)

#########################################################################################
# Package Configuration
#########################################################################################
# Helpers functions for creating config files that can be included by other projects
include(CMakePackageConfigHelpers)

# Create config package
configure_package_config_file(
    "Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/serde_cpp")
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion)

# Install config package
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/serde_cppConfigVersion.cmake"
        DESTINATION "lib/cmake/serde_cpp")
install(EXPORT "serde_cppTargets"
        NAMESPACE "serde_cpp::"
        DESTINATION "lib/cmake/serde_cpp")
