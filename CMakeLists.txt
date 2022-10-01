cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 99)

project(jsonpackerc LANGUAGES C)

# add_subdirectory(src)

add_executable(jsonpackerc ${CMAKE_SOURCE_DIR}/src/cJSON/cJSON.c ${CMAKE_SOURCE_DIR}/src/main.c)
