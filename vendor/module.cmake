
include (ExternalProject)

# ***********************************************
# DocOpt project
# ExternalProject_Add( docopt
#   URL ${CMAKE_CURRENT_LIST_DIR}/docopt
#   BUILD_IN_SOURCE 0
#   INSTALL_DIR ${CMAKE_BINARY_DIR}/lib
#   INSTALL_COMMAND install ${CMAKE_BINARY_DIR}/docopt-prefix/src/docopt-build/libdocopt.a ${CMAKE_BINARY_DIR}/lib
# )


# ***********************************************
# ALAC
#   https://github.com/macosforge/alac
ExternalProject_Add(libalac
  URL ${CMAKE_CURRENT_LIST_DIR}/alac/codec
  BUILD_IN_SOURCE 1
  PREFIX  "libalac"

  CONFIGURE_COMMAND ""
  COMMAND make
  INSTALL_DIR ${CMAKE_BINARY_DIR}/lib
  INSTALL_COMMAND install ${CMAKE_BINARY_DIR}/libalac/src/libalac/libalac.a ${CMAKE_BINARY_DIR}/lib
)

#set(ALAC_LIBS "${CMAKE_BINARY_DIR}/lib/libalac")

# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/mp4v2)
# include_directories(${CMAKE_CURRENT_LIST_DIR}/mp4v2/include(    ))

#add_library(docopt_s STATIC ${CMAKE_BINARY_DIR}/docopt-prefix/src/docopt-build/libdocopt.a)

#  COMMAND make
#  COMMAND install -d ${OUT_DIR}/lib/
#  COMMAND install libalac.a ${OUT_DIR}/lib/libalac.a




#SET_TARGET_PROPERTIES(docopt PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}vendor/docopt/libdocopt.a)

# list(APPEND SOURCES
#     ${CMAKE_CURRENT_LIST_DIR}/informat.h
#     ${CMAKE_CURRENT_LIST_DIR}/informat.cpp

#     ${CMAKE_CURRENT_LIST_DIR}/in_ape.h
#     ${CMAKE_CURRENT_LIST_DIR}/in_ape.cpp

#     ${CMAKE_CURRENT_LIST_DIR}/in_flac.h
#     ${CMAKE_CURRENT_LIST_DIR}/in_flac.cpp

#     ${CMAKE_CURRENT_LIST_DIR}/in_tta.h
#     ${CMAKE_CURRENT_LIST_DIR}/in_tta.cpp

#     ${CMAKE_CURRENT_LIST_DIR}/in_wav.h
#     ${CMAKE_CURRENT_LIST_DIR}/in_wav.cpp

#     ${CMAKE_CURRENT_LIST_DIR}/in_wv.h
#     ${CMAKE_CURRENT_LIST_DIR}/in_wv.cpp

#     ${CMAKE_CURRENT_LIST_DIR}/in_wave64.h
#     ${CMAKE_CURRENT_LIST_DIR}/in_wave64.cpp

# )
