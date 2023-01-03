#########################################################################################
# Tests
#########################################################################################

set(TEST_HEADERS
  mytypes.h
)
set(TEST_SOURCES
  test.cpp
)
list(APPEND TEST_FILES ${TEST_HEADERS} ${TEST_SOURCES})
message(STATUS "TEST_FILES: ${TEST_FILES}")

foreach(FILE IN LISTS TEST_FILES)
  string(REGEX REPLACE "[.](cc|cpp|h|hpp)$" "_serde.h" SERDE_HEADER ${FILE})
  string(PREPEND SERDE_HEADER "${CMAKE_CURRENT_BINARY_DIR}/")
  string(PREPEND FILE "${CMAKE_CURRENT_SOURCE_DIR}/")
  list(APPEND SERDE_HEADERS ${SERDE_HEADER})
  list(APPEND TEST_FILES_PATH ${FILE})
endforeach()

message(STATUS "SERDE_HEADERS: ${SERDE_HEADERS}")
message(STATUS "TEST_FILES_PATH: ${TEST_FILES_PATH}")

file(TOUCH ${SERDE_HEADERS})

add_custom_command(
  OUTPUT ${SERDE_HEADERS}
  COMMAND $<TARGET_FILE:serde_gen> --database_dir=${CMAKE_BINARY_DIR} --database_file=compile_commands.json ${TEST_FILES_PATH}
  DEPENDS serde_gen ${TEST_FILES_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
)
add_custom_target(serde_gen_files ALL DEPENDS ${SERDE_HEADERS})

add_executable(serde_attr_test)
target_sources(serde_attr_test PRIVATE ${TEST_SOURCES})
target_include_directories(serde_attr_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(serde_attr_test PRIVATE serde serde_yaml)
add_dependencies(serde_attr_test serde_gen_files)
target_compile_options(serde_attr_test PRIVATE -Wno-attributes)

