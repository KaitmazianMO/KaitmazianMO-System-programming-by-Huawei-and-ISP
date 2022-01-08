include ("../../cmake/dir_to_lib.cmake")

list(APPEND EXTERNAL_LIBS_LIST tok log err str_view)
list(APPEND DIRS_TO_INCLUDE_LIST "${CMAKE_SOURCE_DIR}/src/Object/")


dir_to_lib (asm )