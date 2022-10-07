#include <jsonp.h>
#include <argparser.h>

int main(int argc, char** argv) {
    int status;
    FILE *outstream;
    JSONpArgs jsonp_args;

    /* Where the magic happens */
    jsonp_argparser (&argc, argv, &jsonp_args);
    // const char *filepath = "../resources/records.txt";
    status = JSONp_Pack(&jsonp_args);
    return status;
}

