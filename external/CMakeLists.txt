cmake_minimum_required(VERSION 3.5)
include(ExternalProject)

#########################################################################################
# GoogleTest
#########################################################################################
# GoogleTest is Google’s C++ testing and mocking framework.
# GoogleTest is licensed under the BSD-3-Clause license.
message(STATUS "Adding external library: GoogleTest")
ExternalProject_Add(googletest_external
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googletest
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DBUILD_SHARED_LIBS=OFF
        -DBUILD_GMOCK=ON
        -DINSTALL_GTEST=ON)

#########################################################################################
# RapidYaml
#########################################################################################
# RapidYaml is a library to parse and emit YAML, and do it fast.
# GoogleTest is licensed under the BSD-3-Clause license.
message(STATUS "Adding external library: RapidYaml")
ExternalProject_Add(rapidyaml_external
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/rapidyaml
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidyaml
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DBUILD_SHARED_LIBS=OFF)

