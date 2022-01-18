cmake_minimum_required(VERSION 3.5)

project(libalac
    VERSION 0.1.0
    DESCRIPTION "Apple Lossless (ALAC) encoder and decoder library"
    LANGUAGES C
)

SET(HEADERS
    codec/EndianPortable.h
    codec/aglib.h
    codec/ALACAudioTypes.h
    codec/ALACBitUtilities.h
    codec/ALACDecoder.h
    codec/ALACEncoder.h
    codec/dplib.h
    codec/matrixlib.h
)

SET(SOURCES
    codec/EndianPortable.c
    codec/ALACBitUtilities.c
    codec/ALACDecoder.cpp
    codec/ALACEncoder.cpp
    codec/ag_dec.c
    codec/ag_enc.c
    codec/dp_dec.c
    codec/dp_enc.c
    codec/matrix_dec.c
    codec/matrix_enc.c
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_definitions(-DTARGET_OS_MAC=1)
endif()

add_library(alac_s STATIC ${HEADERS} ${SOURCES})


