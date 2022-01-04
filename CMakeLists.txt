cmake_minimum_required(VERSION 3.5)

project(alacenc
    VERSION 0.1.0
    DESCRIPTION "command-line ALAC encoder"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

LIST(APPEND SOURCES
    main.cpp

    types.h
    types.cpp

    wavheader.h
    wavheader.cpp

    encoder.h
    encoder.cpp
)


add_definitions(-DPROJECT_NAME=\"${PROJECT_NAME}\")
add_definitions(-DPROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\")
add_definitions(-DPROJECT_DESCRIPTION=\"${PROJECT_DESCRIPTION}\")


include(vendor/module.cmake)

link_directories(${CMAKE_BINARY_DIR}/lib)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} alac)



