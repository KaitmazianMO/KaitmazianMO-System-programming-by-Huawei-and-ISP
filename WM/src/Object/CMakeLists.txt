include ("../../cmake/dir_to_lib.cmake")

list(APPEND EXTERNAL_LIBS_LIST log err)

dir_to_lib(obj "${EXTERNAL_LIBS_LIST}" "")