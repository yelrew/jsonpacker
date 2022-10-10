#include "unity.h"
#include "jsonp.h"

static void invalid_option_from_command_line(void) {

    int status;
    int argc = 3;
    char *argv[] = {"jsonpacker",
                    "records.txt",
                    "--bad-option"};
    JSONpArgs jsonp_args;

    TEST_ASSERT(jsonp_argparser (&argc, argv, &jsonp_args, ARGP_SILENT) != JSONP_SUCCESS);
}

int test_cljsonp() {

    UNITY_BEGIN();
    RUN_TEST(invalid_option_from_command_line);
    return UNITY_END();

}
