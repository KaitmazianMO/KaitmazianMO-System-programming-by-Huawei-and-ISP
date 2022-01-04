include ("../../cmake/dir_to_lib.cmake")

list(APPEND EXTERNAL_LIBS_LIST log err str_view)
list(APPEND DIRS_TO_INCLUDE_LIST "${CMAKE_SOURCE_DIR}/src/Object/")

dir_to_lib(lex "${EXTERNAL_LIBS_LIST}" "${DIRS_TO_INCLUDE_LIST}")

target_include_directories(lex PRIVATE "${CMAKE_SOURCE_DIR}/src/Object/")