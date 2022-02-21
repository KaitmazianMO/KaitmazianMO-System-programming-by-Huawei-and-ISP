cmake_minimum_required(VERSION 3.16)
project(WM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra")

add_subdirectory("src/Log/")
add_subdirectory("src/Error/")
add_subdirectory("src/Strings/")
add_subdirectory("src/Lexer/")
add_subdirectory("src/Object")
add_subdirectory("src/ObjectPool")
add_subdirectory("src/Meta")
#add_subdirectory("src/WASM")

add_subdirectory("unit-tests")