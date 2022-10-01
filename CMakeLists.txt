cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 99)
# SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --g")


project(jsonpackerc LANGUAGES C)

# add_subdirectory(src)

add_executable(jsonpackerc ${CMAKE_SOURCE_DIR}/src/cJSON/cJSON.c ${CMAKE_SOURCE_DIR}/src/main.c)

include_directories(${CMAKE_SOURCE_DIR}/src)
