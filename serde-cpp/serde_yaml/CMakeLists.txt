#########################################################################################
# Dependencies
#########################################################################################
# GoogleTest for unit testing
find_package(GTest REQUIRED)
# RapidYaml for parsing and emitting YAML
find_package(ryml REQUIRED)
find_package(c4core REQUIRED)

#########################################################################################
# serde_yaml
#########################################################################################
add_library(serde_yaml STATIC)
target_sources(serde_yaml PRIVATE
  src/serializer_yaml.cpp
  src/deserializer_yaml.cpp
)
target_include_directories(serde_yaml PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(serde_yaml
  PUBLIC serde
  PRIVATE
  $<BUILD_INTERFACE:ryml::ryml>
  #ryml::ryml
  #c4core::c4core
)
add_custom_command(TARGET serde_yaml POST_BUILD
    COMMAND libtool --tag=serde_yaml --mode=link cc -static -o $<TARGET_FILE:serde_yaml>
    $<TARGET_FILE:serde_yaml> $<TARGET_FILE:ryml::ryml> $<TARGET_FILE:c4core::c4core>
)
install(TARGETS serde_yaml EXPORT serde_cppTargets)
install(DIRECTORY include/serde_yaml DESTINATION include)

#########################################################################################
# Tests
#########################################################################################
add_executable(serde_yaml_test)
target_sources(serde_yaml_test PRIVATE
  test/advanced.cpp
  test/std.cpp
  test/types.cpp
  test/errors.cpp
  test/builtin.cpp
)
target_include_directories(serde_yaml_test PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(serde_yaml_test PRIVATE
  serde_yaml
  GTest::gmock_main
  GTest::gmock
  GTest::gtest
)

