// #include <jsonp.h>
#include <argp.h>
#include <argparser.h>
// #include <stdio.h>

/* Command-line arguments */
static char args_doc[] = "<JSON file>";

static struct argp_option options[] =
{
  {"output",  0 , "outfile", OPTION_ARG_OPTIONAL,
   "Changes default file output name (optional)"},
  {0}
};

static char doc[] =
    "JSON Packer -- A program to encode JSON key-valued objects in binary"
    "format.\v"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions.\n"
    "Copyright (C) 2022  Eduardo Werley.\n\n";


static struct argp argp = {options, parse_opt, args_doc, doc};

const char *argp_program_version;

int argparser(int *argc, char** argv, Arguments *arguments) {

    /* Set argument defaults */
    FILE *outstream;
    arguments->outfile = NULL;

    char argp_program_version_full[80];
    sprintf(argp_program_version_full, \
            "JSON Packer %d.%d", \
            JSONP_VERSION_MAJOR, JSONP_VERSION_MINOR);
    argp_program_version = argp_program_version_full;

    /* Where the magic happens */
    argp_parse (&argp, *argc, argv, 0, 0, arguments);

    /* Where do we send output? */
//    if (arguments.outfile)
//        outstream = fopen (arguments.outfile, "w");
//    else
//        outstream = stdout;

    return 0;

}

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    Arguments *arguments = state->input;
    switch (key) {
    case 'o':
        arguments->outfile = arg;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            argp_usage(state);
        arguments->args[state->arg_num] = arg;
        arguments->infile = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1)
            argp_usage (state);
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}