#########################################################################################
# Tests
#########################################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/serde_gen/cmake")
include(serde-cpp)

serde_generate_target(test_serde_files
  mytypes.h
  test.cpp
)

add_executable(serde_gen_test)
target_sources(serde_gen_test PRIVATE
  test.cpp
)
target_link_libraries(serde_gen_test PRIVATE
  test_serde_files
  serde_yaml
  serde
)
target_compile_options(serde_gen_test PRIVATE -Wno-attributes)

