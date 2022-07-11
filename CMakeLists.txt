cmake_minimum_required(VERSION 3.5)
project(serde LANGUAGES CXX)

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
# Project
#########################################################################################
add_executable(serde_test)
target_sources(serde_test PRIVATE
  src/test.cpp
)
target_include_directories(serde_test PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)
#target_link_libraries(serde PRIVATE)
#target_compile_definitions(spaceship PRIVATE)
#install(TARGETS spaceship)
