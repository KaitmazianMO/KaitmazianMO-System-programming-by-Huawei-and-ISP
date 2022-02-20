include ("../../cmake/dir_to_lib.cmake")

list(APPEND EXTERNAL_LIBS_LIST log err)
list(APPEND DIRS_TO_INCLUDE_LIST "${CMAKE_SOURCE_DIR}/src/Object/")

dir_to_lib(obj_pool "${EXTERNAL_LIBS_LIST}" "${DIRS_TO_INCLUDE_LIST}")