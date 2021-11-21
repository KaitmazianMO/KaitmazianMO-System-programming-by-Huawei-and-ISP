cmake_minimum_required(VERSION 3.16)

include (FetchContent)
FetchContent_Declare (
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable (googletest)

enable_testing()

add_subdirectory (../ListLib)
include_directories (../ListLib)

add_executable (
    list_test
    test.cpp
)

target_link_libraries (
    list_test
    CacheFriendlyList
    Log
    Graphviz
    gtest_main
)

include (GoogleTest)
gtest_discover_tests (list_test)

add_dependencies (list_test CacheFriendlyList)