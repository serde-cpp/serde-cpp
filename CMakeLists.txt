cmake_minimum_required(VERSION 3.5)
project(serde-cpp)

# This is a Super-Project which builds Sub-Projects and manage library dependencies.
# Libraries are built first as external projects and installed at the build directory.
# Sub-Projects are built after and can relate to the pre-built external libraries in the
# build directory with find_package or find_library.

#########################################################################################
# Configuration
#########################################################################################
# Default build type to Debug, for release use `cmake -DCMAKE_BUILD_TYPE=Release ..`
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type default to Debug" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#########################################################################################
# Dependencies
#########################################################################################
# Module ExternaProject for building projects in external trees.
include(ExternalProject)
# Include the external libraries in the path where cmake looks for find_package/library.
list(PREPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/external")
# `external` directory contains all dependency libraries for the sub-projects.
add_subdirectory(external)

#########################################################################################
# Project
#########################################################################################
message(STATUS "Adding sub-project: serde-cpp")
ExternalProject_Add(serde-cpp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/serde-cpp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/serde-cpp
    BUILD_ALWAYS 1
    CMAKE_ARGS
        -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    DEPENDS
        rapidyaml_external
        googletest_external)

