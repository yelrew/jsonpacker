cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_C_STANDARD 99)

set(PROJECT_NAME jsonpacker)
project("${PROJECT_NAME}" LANGUAGES C VERSION 1.0.0)

option(WITH_SPHINX "Generate Sphinx Documentation")

FIND_PATH(
  APR_INCLUDE_DIR
    apr.h apr_general.h
  HINTS
    /usr/include/apr-1
    /usr/include/apr-1.0
)

include_directories(jsonpacker ${CMAKE_SOURCE_DIR}/src
    "${APR_INCLUDE_DIR}")

# target_include_directories(jsonpacker INTERFACE apr-1)
# SET(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")

set(target_sources
    cljsonp.c
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

add_executable(jsonpacker "${target_sources}")
target_link_libraries(jsonpacker apr-1)

find_package(Doxygen OPTIONAL_COMPONENTS dot)
find_package(Sphinx)


if (DOXYGEN_FOUND)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
    set(DOXYGEN_PROJECT_BRIEF  "A JSON binary packer")
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
#  set(PROJECT_NUMBER 1.0.0)
    set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_SOURCE_DIR}/src/cJSON")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen")

   if(SPHINX_FOUND)
       if (WITH_SPHINX)

           configure_file(
               "${CMAKE_SOURCE_DIR}/doc/conf.py.in"
               "${CMAKE_SOURCE_DIR}/doc/conf.py"
               @ONLY)

            set(DOXYGEN_GENERATE_XML YES)
            set(DOXYGEN_GENERATE_HTML NO) # Sphinx needs xml only

            set(SPHINX_SOURCE ${CMAKE_SOURCE_DIR}/doc)
            set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/doc/sphinx)

            set (doxygen_target "doxygen")
            set (sphinx_target "doc")
        else()
            set (doxygen_target "doc")
            set (sphinx_target "Sphinx")
        endif()

        add_custom_target(${sphinx_target}
                          COMMAND ${SPHINX_EXECUTABLE} -b html
                          ${SPHINX_SOURCE} ${SPHINX_BUILD}
                          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                          COMMENT "Generating Sphinx documentation")
        add_dependencies(${sphinx_target} ${doxygen_target})

    else ()
        message("Sphinx not found!")
    endif()

    message("Doxygen project number is ${DOXYGEN_PROJECT_NUMBER}")
    doxygen_add_docs(${doxygen_target}
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/README.md"
    )

else()

    message("Doxygen not found!")

endif(DOXYGEN_FOUND)


option(ENABLE_JSONP_TEST "Enable building JSONp test" ON)

if (ENABLE_JSONP_TEST)
    enable_testing()
    add_subdirectory("${CMAKE_SOURCE_DIR}/tests")
endif()


