include ("../../cmake/dir_to_lib.cmake")

list(APPEND EXTERNAL_LIBS_LIST obj log err)

dir_to_lib(obj_pool "${EXTERNAL_LIBS_LIST}" "${DIRS_TO_INCLUDE_LIST}")