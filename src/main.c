#include <jsonp.h>
#include <argp.h>
#include <stdio.h>
#include <argparser.h>

int main(int argc, char** argv) {
    int status;
    FILE *outstream;
    Arguments arguments;

    /* Where the magic happens */
    argparser (&argc, argv, &arguments);
    // const char *filepath = "../resources/records.txt";
    status = JSONp_Pack(arguments.infile);
    return status;
}

