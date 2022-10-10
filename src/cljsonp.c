#include <jsonp.h>
#include <argparser.h>

int main(int argc, char** argv) {
    int status;
    JSONpArgs jsonp_args;

    assert(jsonp_argparser (&argc, argv, &jsonp_args, 0) == JSONP_SUCCESS);
    status = JSONp_Pack(&jsonp_args);

    return status;
}

