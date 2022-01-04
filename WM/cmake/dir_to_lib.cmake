
function(dir_to_lib LIB_NAME EXTERNAL_LIBS DIRECTORIES_TO_INCLUDE)
    file(GLOB SOURCES 
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    add_library (${LIB_NAME} STATIC ${SOURCES})
    
    target_link_libraries (${LIB_NAME} PRIVATE "${EXTERNAL_LIBS}")
    
    target_include_directories(
        ${LIB_NAME}
        INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
        PRIVATE   "${DIRECTORIES_TO_INCLUDE}"
    )
endfunction()