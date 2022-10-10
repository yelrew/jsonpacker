if(ENABLE_JSONP_TEST)

    add_library(unity STATIC Unity/src/unity.c)

    add_library(libjsonpacker "${target_sources}")
    target_link_libraries(libjsonpacker apr-1)


#    target_include_directories(unity PUBLIC Unity/src)

    #    set(unity_tests
    #        test_jsonp_asn1
    #    )

    set(unity_tests
        test_first.c
        test_first_unity.c
        test_jsonp.c
        test_jsonp_asn1.c
        test_cljsonp.c
    )

    create_test_sourcelist (DriverUnityTests
        driver.c # this
        ${unity_tests}
    )

    # add the executable (Just one executable!)
    add_executable (unity_tests ${DriverUnityTests} unity_setup.c)
    # add_executable (unity_tests ${DriverUnityTests})
    target_include_directories(unity_tests PUBLIC
      ${CMAKE_SOURCE_DIR}/tests/Unity/src
    )

    target_link_libraries(unity_tests unity libjsonpacker)

    # remove the test driver source file
    #      set (TestsToRun ${Tests})
    #      remove (TestsToRun driver.c)

    # Add all the ADD_TEST for each test
    # foreach (test ${TestsToRun})

#    add_test (NAME setup COMMAND unity_tests setup.c)
#    add_test (NAME cleanup COMMAND unity_tests teardown.c)
#    set_tests_properties(setup PROPERTIES FIXTURES_SETUP DB)
#    set_tests_properties(cleanup PROPERTIES FIXTURES_CLEANUP DB)

    foreach (test ${unity_tests})
        get_filename_component (TestName ${test} NAME_WE)
        add_test (NAME ${TestName} COMMAND unity_tests ${TestName})
        # set_tests_properties(${TestName} PROPERTIES DEPENDS unity_setup.c)

#        set_tests_properties(${TestName} PROPERTIES FIXTURES_REQUIRED DB)

#        set_tests_properties(setUp         PROPERTIES FIXTURES_SETUP    Db)
#        set_tests_properties(tearDown       PROPERTIES FIXTURES_CLEANUP  Db)

#        // msvc doesn't support weak-linking, so we need to define these functions.
#        void setUp(void) { }
#        void tearDown(void) { }

#        set_tests_properties(${TestName} PROPERTIES
#          FIXTURES_SETUP    Foo
#          FIXTURES_REQUIRED Bar
#        )
    endforeach ()

#      foreach(unity_test ${unity_tests})
#          add_executable("${unity_test}" "${unity_test}.c")
#          target_link_libraries("${unity_test}" unity)

#          add_test(NAME "${unity_test}" COMMAND "./${unity_test}"
#      )

#      endforeach()

#    foreach(unity_test ${unity_tests})
#        add_executable("${unity_test}" "${unity_test}.c")
##        target_link_libraries("${unity_test}" "${CJSON_LIB}" unity)
#        target_link_libraries("${unity_test}" unity)

#        add_test(NAME "${unity_test}" COMMAND "./${unity_test}"
#    )

#    endforeach()

endif()
