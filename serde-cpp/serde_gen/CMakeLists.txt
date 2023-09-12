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
  src/gen.cpp
)
target_link_libraries(serde_gen PRIVATE
  serde
  cppast
  cxxopts::cxxopts
)
install(TARGETS serde_gen)

#########################################################################################
# Tests
#########################################################################################
add_subdirectory(test)
