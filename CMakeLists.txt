cmake_minimum_required(VERSION 3.5)

project(alacenc
    VERSION 0.4.0
    DESCRIPTION "command-line ALAC encoder"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set ( CMAKE_BUILD_TYPE Release )
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG ${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-g ${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG ${CMAKE_C_FLAGS_RELEASE}")


if (CMAKE_BUILD_TYPE STREQUAL Release)
  message("For building debug version use -DCMAKE_BUILD_TYPE=Debug option.")
endif()

LIST(APPEND SOURCES
    main.cpp

    types.h
    types.cpp

    wavheader.h
    wavheader.cpp

    encoder.h
    encoder.cpp

    atoms.h
    atoms.cpp

    tags.h
    tags.cpp
)


# Defining the bit order for different architectures
if(CMAKE_VERSION VERSION_LESS "3.20.0")
    include(TestBigEndian)
    TEST_BIG_ENDIAN(big_endian)
    if (${big_endian})
        set(TARGET_RT_LITTLE_ENDIAN 0)
    else()
        set(TARGET_RT_LITTLE_ENDIAN 1)
    endif()
else()
    if (${CMAKE_CXX_BYTE_ORDER} STREQUAL "LITTLE_ENDIAN")
        set(TARGET_RT_LITTLE_ENDIAN 1)
    else()
        set(TARGET_RT_LITTLE_ENDIAN 0)
    endif()
endif()

if (${TARGET_RT_LITTLE_ENDIAN})
    message(STATUS "Use little endian")
else()
    message(STATUS "Use big endian")
endif()

add_definitions(-DTARGET_RT_LITTLE_ENDIAN=${TARGET_RT_LITTLE_ENDIAN})
# Defining the bit order for different architectures

add_definitions(-DPROJECT_NAME=\"${PROJECT_NAME}\")
add_definitions(-DPROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\")
add_definitions(-DPROJECT_DESCRIPTION=\"${PROJECT_DESCRIPTION}\")


add_subdirectory(vendor/alac EXCLUDE_FROM_ALL)
add_subdirectory(vendor/docopt EXCLUDE_FROM_ALL)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} alac_s docopt_s)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)


