include_directories(${CMAKE_BINARY_DIR})

file(
  GLOB_RECURSE TEST_HEADERS
  LIST_DIRECTORIES false */*.hpp
)
file(
  GLOB_RECURSE TEST_SOURCES
  LIST_DIRECTORIES false unittests.cpp cpp/*/*.cpp cpp/*/*/*.cpp
)

add_executable(unittests ${TEST_SOURCES})

target_compile_definitions(
  unittests
  PRIVATE
    DATA_PATH="${PROJECT_SOURCE_DIR}/data/"
)

target_include_directories(
  unittests
  PUBLIC
    TEST_HEADERS
)
target_link_libraries(
  unittests
  PUBLIC
    Fields2Cover
    GTest::gtest GTest::gtest_main
)

add_test(
  NAME unittests
  COMMAND unittests
  WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
add_dependencies(check unittests)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(GCOVR_ADDITIONAL_ARGS
    --exclude-throw-branches
    --exclude-function-lines
    --exclude-unreachable-branches
    -x coverage_xml.xml
  )
  setup_target_for_coverage_gcovr_html(
    NAME
      coverage_unittests
    EXECUTABLE
      ${CMAKE_BINARY_DIR}/tests/unittests
    EXCLUDE
      ${PROJECT_SOURCE_DIR}/tests
      ${PROJECT_SOURCE_DIR}/build
  )
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
