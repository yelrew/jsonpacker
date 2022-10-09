if(ENABLE_JSONP_TEST)

    add_library(unity STATIC Unity/src/unity.c)

    #    set(unity_tests
    #        test_jsonp_asn1
    #    )

    set(unity_tests
        test_first.c
        test_first_unity.c
        test_jsonp_asn1.c
    )

    create_test_sourcelist (DriverUnityTests
        driver.c # this
        ${unity_tests}
        #        test_first.c
        #        test_first_unity.c
        #        test_jsonp_asn1.c
    )

    # add the executable (Just one executable!)
    add_executable (unity_tests ${DriverUnityTests})

    # remove the test driver source file
    #      set (TestsToRun ${Tests})
    #      remove (TestsToRun driver.c)

    # Add all the ADD_TEST for each test
    # foreach (test ${TestsToRun})

    foreach (test ${unity_tests})
        get_filename_component (TestName ${test} NAME_WE)
        add_test (NAME ${TestName} COMMAND unity_tests ${TestName})
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
