function (dir_to_lib LIB_NAME LIB_TYPE)
    file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    add_library (${LIB_NAME} ${LIB_TYPE} ${SOURCES})

    if (${ARGC} GREATER 2)
        target_link_libraries (${LIB_NAME} PRIVATE {${ARGN}})
    endif()
endfunction()