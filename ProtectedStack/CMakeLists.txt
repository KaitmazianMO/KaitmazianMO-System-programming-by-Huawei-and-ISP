cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(POJECT_NAME ProtectedStack)

project(${POJECT_NAME})

add_compile_options(-Werror -finstrument-functions -rdynamic -ldl -lbfd -O2)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    Canary.cpp
    gcc_trace.cpp
    hash.cpp
    Stack.cpp
    ProtectedBuffer.cpp
    Memory.cpp
    ProtectedBuffer.cpp
)

add_library(
    Log STATIC
    ../Utilities/Log/log.cpp
    ../Utilities/Log/gcc_trace.cpp
)

find_library(dl NAMES dl PATH /usr/lib/x86_64-linux-gnu/libdl.so)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC Log ${dl}
)
