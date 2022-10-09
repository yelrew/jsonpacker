cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 99)
# SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --g")

project(jsonpackerc LANGUAGES C)

FIND_PATH(
  APR_INCLUDE_DIR
    apr.h apr_general.h
  HINTS
    /usr/include/apr-1
    /usr/include/apr-1.0
)

include_directories(jsonpackerc ${CMAKE_SOURCE_DIR}/src
                    "${APR_INCLUDE_DIR}")

# target_include_directories(jsonpackerc INTERFACE apr-1)
# SET(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")


set(target_sources
    main.c
    jsonp.h
    jsonp.c
    asn1.c
    asn1.h
    argparser.c
    argparser.h
    cJSON/cJSON.h
    cJSON/cJSON.c
)

list(TRANSFORM target_sources PREPEND "${CMAKE_SOURCE_DIR}/src/")

add_executable(jsonpackerc "${target_sources}")
target_link_libraries(jsonpackerc apr-1)

find_package(Doxygen OPTIONAL_COMPONENTS dot)
if (DOXYGEN_FOUND)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
  set(DOXYGEN_PROJECT_BRIEF  "A JSON binary packer")
  set(PROJECT_NAME "JSON Packer")
#  set(PROJECT_NUMBER 1.0.0)
  set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_SOURCE_DIR}/src/cJSON")

  doxygen_add_docs(doc
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/README.md"
    COMMENT "Generating API documentation with Doxygen"
  )
endif(DOXYGEN_FOUND)
