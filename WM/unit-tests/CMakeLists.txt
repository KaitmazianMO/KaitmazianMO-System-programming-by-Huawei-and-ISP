enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_lex_target test_lex.cpp)
target_include_directories(test_lex_target PRIVATE "${CMAKE_SOURCE_DIR}/src/lex/")
target_link_libraries(test_lex_target lex GTest::Main)
add_test(NAME test_lex COMMAND test_lex_target)
gtest_discover_tests(test_lex_target)

add_executable(test_obj_pool_target test_obj_pool.cpp)
target_link_libraries(test_obj_pool_target obj_pool GTest::Main)
add_test(NAME test_obj_pool COMMAND test_obj_pool_target)
gtest_discover_tests(test_obj_pool_target)
