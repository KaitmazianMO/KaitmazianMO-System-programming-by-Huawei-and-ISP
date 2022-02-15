cmake_minimum_required (VERSION 3.16)

add_library (
    CacheFriendlyList
    STATIC
    list.h
    list.cpp
)

target_link_libraries (
    CacheFriendlyList
    Log
)

target_include_directories(
    CacheFriendlyList
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)