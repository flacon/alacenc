cmake_minimum_required(VERSION 3.5)

project(alacenc
    VERSION 0.0.1
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

    atoms.h
    atoms.cpp

    tags.h
    tags.cpp
)


add_definitions(-DPROJECT_NAME=\"${PROJECT_NAME}\")
add_definitions(-DPROJECT_VERSION=\"${CMAKE_PROJECT_VERSION}\")
add_definitions(-DPROJECT_DESCRIPTION=\"${PROJECT_DESCRIPTION}\")


add_subdirectory(vendor/alac EXCLUDE_FROM_ALL)
add_subdirectory(vendor/docopt EXCLUDE_FROM_ALL)


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} alac_s docopt_s)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)


