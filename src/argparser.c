#include <jsonp.h>
#include <argparser.h>

/* Command-line arguments */
static char args_doc[] = "<JSON file>";

static struct argp_option options[] =
{
  {"encoder",  'e' , "encoder", OPTION_ARG_OPTIONAL,
   "Changes default file output name (optional)"},
  {"output",  'o' , "outfile", OPTION_ARG_OPTIONAL,
   "Changes default file output name (optional)"},
  {0}
};

static char doc[] =
    "JSON Packer -- A program to encode JSON key-valued objects in binary"
    " format.\v"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions."
    "Copyright (C) 2022  Eduardo Werley.\n\n";

static struct argp argp = {options, parse_opt, args_doc, doc};

const char *argp_program_version;

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    JSONpArgs *arguments = state->input;
    switch (key) {
    case 'e':
        assert(strlen(arg) < JSONP_MAX_ENCNAME_LENGTH);
        strncpy(arguments->encoder_name, arg, JSONP_MAX_ENCNAME_LENGTH - 1);
        arguments->encoder_name[JSONP_MAX_ENCNAME_LENGTH - 1] = '\0';
        if (strcmp(arg, "JpASN1") == 0)
            arguments->encoder = JpASN1;
        else if (strcmp(arg, "GnuASN1") == 0)
            arguments->encoder = GnuASN1;
        else if (strcmp(arg, "libBSON") == 0)
            arguments->encoder = libBSON;
        break;
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

int jsonp_argparser(int *argc, char** argv, JSONpArgs *jsonp_args) {

    /* Set argument defaults */
    // FILE *outstream;
    jsonp_args->outfile = NULL;
    jsonp_args->encoder = JpASN1;

    char argp_program_version_full[80];
    sprintf(argp_program_version_full, \
            "JSON Packer %d.%d", \
            JSONP_VERSION_MAJOR, JSONP_VERSION_MINOR);
    argp_program_version = argp_program_version_full;

    /* Where the magic happens */
    argp_parse (&argp, *argc, argv, 0, 0, jsonp_args);

    /* Where do we send output? */
//    if (arguments.outfile)
//        outstream = fopen (arguments.outfile, "w");
//    else
//        outstream = stdout;

    return 0;

}
