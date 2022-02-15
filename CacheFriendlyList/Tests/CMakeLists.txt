find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

add_executable (
    list_test
    test.cpp
)

target_compile_options (
  list_test PRIVATE "-g"
)

target_link_libraries (
    list_test
    CacheFriendlyList
    Log
    Graphviz
    GTest::Main
)

include (GoogleTest)
gtest_discover_tests (list_test)

add_dependencies (list_test CacheFriendlyList)

gtest_discover_tests(list_test)