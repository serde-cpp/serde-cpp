#########################################################################################
# Tests
#########################################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/serde_attr/cmake")
include(serde-cpp)

serde_generate_target(test_serde_files
  ${CMAKE_CURRENT_SOURCE_DIR}/mytypes.h
  test.cpp
  #../test/other/test.cpp
  #../test.cpp
  #hello
  #t2.cc
  #.hi.h
  #.fsfd
)

add_executable(serde_attr_test)
target_sources(serde_attr_test PRIVATE
  test.cpp
)
target_link_libraries(serde_attr_test PRIVATE serde serde_yaml test_serde_files)
target_compile_options(serde_attr_test PRIVATE -Wno-attributes)

