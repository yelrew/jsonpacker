cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 99)
# SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --g")

project(jsonpackerc LANGUAGES C)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(target_sources
    main.c
    json_packer.h
    json_packer.c
    cJSON/cJSON.h
    cJSON/cJSON.c
)

list(TRANSFORM target_sources PREPEND "${CMAKE_SOURCE_DIR}/src/")

add_executable(jsonpackerc "${target_sources}")
