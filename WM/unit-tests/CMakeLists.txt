enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
gtest_discover_tests(${TEST_TARGET})

add_executable(test_lex_target test_lex.cpp)
target_include_directories(test_lex_target "${CMAKE_SOURCE_DIR}/src/lex/")
target_link_libraries(test_lex lex GTest::Main)
add_test(NAME test_lex COMMAND test_lex_target)