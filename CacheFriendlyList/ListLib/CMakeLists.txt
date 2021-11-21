cmake_minimum_required (VERSION 3.16)

add_library (
    CacheFriendlyList
    list.h
    list.cpp
)

add_subdirectory (../Log)

target_link_libraries (
    CacheFriendlyList
    Log
)