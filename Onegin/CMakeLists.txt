cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(POJECT_NAME Onegin)

project(${POJECT_NAME})

add_compile_options(-Werror -finstrument-functions -rdynamic -ldl -lbfd -O2)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    buffer.cpp
    onegin_text.cpp
    text.cpp
    ../Utilities/stdlib_addition.cpp
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

#target_link_libraries(${PROJECT_NAME} PUBLIC ${dl})