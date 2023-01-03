#########################################################################################
# Tests
#########################################################################################

set(SOURCES
  test.cpp
)

string(REGEX REPLACE "[.]cpp$" "_serde.h" SERDE_HEADERS ${SOURCES})
string(PREPEND SERDE_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/")
string(PREPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/")
message(STATUS "SERDE_HEADERS: ${SERDE_HEADERS}")

file(TOUCH ${SERDE_HEADERS})

add_custom_command(
  OUTPUT ${SERDE_HEADERS}
  COMMAND ${CMAKE_BINARY_DIR}/serde_attr/serde_gen
          -I ${CMAKE_SOURCE_DIR}/serde/include
          -I ${CMAKE_SOURCE_DIR}/serde_yaml/include
          ${SOURCES}
  DEPENDS serde_gen
          ${SOURCES}
)

add_custom_target(serde_gen_files ALL
  DEPENDS ${SERDE_HEADERS}
)

add_executable(serde_attr_test)
target_sources(serde_attr_test PRIVATE
  ${SOURCES}
)
target_include_directories(serde_attr_test PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(serde_attr_test PRIVATE
  serde
  serde_yaml
)
add_dependencies(serde_attr_test
  serde_gen_files
)
target_compile_options(serde_attr_test PRIVATE -Wno-attributes)
