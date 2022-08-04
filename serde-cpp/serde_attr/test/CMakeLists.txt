#########################################################################################
# Tests
#########################################################################################

add_custom_command(
  OUTPUT test_serde.cpp
  COMMAND ${CMAKE_BINARY_DIR}/serde_attr/serde_gen
          -I ${CMAKE_SOURCE_DIR}/serde/include
          -I ${CMAKE_SOURCE_DIR}/serde_yaml/include
          ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
  DEPENDS serde_gen
)

add_executable(serde_attr_test)
target_sources(serde_attr_test PRIVATE
  test.cpp
  test_serde.cpp
)
target_link_libraries(serde_attr_test PRIVATE
  serde
  serde_yaml
)
target_compile_options(serde_attr_test PRIVATE -Wno-attributes)

