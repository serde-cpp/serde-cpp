#########################################################################################
# Dependencies
#########################################################################################
find_package(cxxopts)
# CppAST to parse and work with the C++ AST
#set(CPPAST_BUILD_TOOL ON) # fetches cxxopts
add_subdirectory(cppast)

#########################################################################################
# serde_gen
#########################################################################################
add_executable(serde_gen
  src/main.cpp
  src/init.cpp
  src/common.cpp
  src/generate.cpp
)
add_executable(serde_cpp::serde_gen ALIAS serde_gen)
target_link_libraries(serde_gen PRIVATE
  serde
  cppast
  cxxopts::cxxopts
)

#########################################################################################
# Installs
#########################################################################################
install(TARGETS serde_gen EXPORT serde_cppTargets)
install(FILES cmake/serde_cpp.cmake DESTINATION lib/cmake/serde_cpp)

#########################################################################################
# Scripts
#########################################################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/serde_gen/cmake")
include(serde_cpp)

#########################################################################################
# Tests
#########################################################################################
add_subdirectory(test)
