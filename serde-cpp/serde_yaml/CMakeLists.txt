#########################################################################################
# Dependencies
#########################################################################################
# GoogleTest for unit testing
find_package(GTest REQUIRED)
# RapidYaml for parsing and emitting YAML
find_package(ryml REQUIRED)

#########################################################################################
# serde_yaml
#########################################################################################
add_library(serde_yaml STATIC)
target_sources(serde_yaml PRIVATE
  src/serializer_yaml.cpp
  src/deserializer_yaml.cpp
)
target_include_directories(serde_yaml PUBLIC include)
target_link_libraries(serde_yaml
  PUBLIC serde
  PRIVATE ryml::ryml
)
install(TARGETS serde_yaml)

#########################################################################################
# Tests
#########################################################################################
add_executable(serde_yaml_test)
target_sources(serde_yaml_test PRIVATE
  test/test.cpp
  test/impl.cpp
)
target_include_directories(serde_yaml_test PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(serde_yaml_test PRIVATE
  serde_yaml
  ryml::ryml
  GTest::gmock_main
  GTest::gmock
  GTest::gtest
)

