cmake_minimum_required(VERSION 3.5)
project(serde-cpp)

# This is a Super-Project which builds Sub-Projects and manage library dependencies.
# Libraries are built first as external projects and installed at the build directory.
# Sub-Projects are built after and can relate to the pre-built external libraries in the
# build directory with find_package or find_library.

#########################################################################################
# Configuration
#########################################################################################
option(SERDE_CPP_INSTALL OFF)

# Check whether this project is a standalone build or part of another project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
set(STANDALONE TRUE)
message(STATUS "Configuring serde-cpp: Standalone project")
else()
set(STANDALONE FALSE)
message(STATUS "Configuring serde-cpp: Subdirectory project")
endif()

# Serde-cpp installation directory
if(SERDE_CPP_INSTALL AND STANDALONE)
set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
else()
set(INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Default build type to Debug, for release use `cmake -DCMAKE_BUILD_TYPE=Release ..`
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type default to Debug" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE} (${PROJECT_NAME})")

#########################################################################################
# Dependencies
#########################################################################################
# Module ExternaProject for building projects in external trees.
include(ExternalProject)
# Include the external libraries in the path where cmake looks for find_package/library.
list(PREPEND PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/external")
# `external` directory contains all dependency libraries for the sub-projects.
add_subdirectory(external)

#########################################################################################
# Sub-Project
#########################################################################################
message(STATUS "Adding sub-project: serde-cpp")
ExternalProject_Add(serde-cpp
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/serde-cpp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/serde-cpp
    INSTALL_DIR ${INSTALL_DIR}
    BUILD_ALWAYS ${STANDALONE}
    CMAKE_ARGS
        -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_PREFIX_PATH=${PREFIX_PATH}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    DEPENDS
        rapidyaml_external
        googletest_external)

#########################################################################################
# Re-export sub-project's targets for add_subdirectory use case
#########################################################################################
if(NOT STANDALONE)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

message(STATUS "Imported target: serde_cpp::serde")
add_library(serde_cpp::serde INTERFACE IMPORTED GLOBAL)
target_include_directories(serde_cpp::serde INTERFACE ${INSTALL_DIR}/include)
add_dependencies(serde_cpp::serde serde-cpp)

message(STATUS "Imported target: serde_cpp::serde_yaml")
add_library(serde_cpp::serde_yaml STATIC IMPORTED GLOBAL)
set_target_properties(serde_cpp::serde_yaml PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libserde_yaml.a)
target_include_directories(serde_cpp::serde_yaml INTERFACE ${INSTALL_DIR}/include)
add_dependencies(serde_cpp::serde_yaml serde-cpp)

endif(NOT STANDALONE)

