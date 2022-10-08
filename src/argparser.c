#include <jsonp.h>
#include <argparser.h>

/* Command-line arguments */
static char args_doc[] = "<JSON file>";

static struct argp_option options[] =
{
  {"encoder",  'e' , "encoder", 0,
   "Changes default file output name (optional)"},
  {"output",  'o' , "outfile", 0,
   "Changes default file output name (optional)"},
  {"one-dict",  'd' , NULL , OPTION_ARG_OPTIONAL,
   "Use a single dicionary for all records (optional)"},
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
        /* Retrieving encoder enum name */
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
    case 'd':
        arguments->one_dict = true;
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

void jsonp_argparser(int *argc, char** argv, JSONpArgs *jsonp_args) {

    /* Set argument defaults */

    jsonp_args->outfile = NULL;
    jsonp_args->encoder = JpASN1;
    jsonp_args->one_dict = false;
    strcpy(jsonp_args->encoder_name, "JpASN1");

    char argp_program_version_full[80];
    sprintf(argp_program_version_full, \
            "JSON Packer %d.%d", \
            JSONP_VERSION_MAJOR, JSONP_VERSION_MINOR);
    argp_program_version = argp_program_version_full;

    argp_parse (&argp, *argc, argv, 0, 0, jsonp_args);

}
