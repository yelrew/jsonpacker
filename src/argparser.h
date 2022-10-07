/** @file argparser.h
 * Function for command-line argument passing
 */

#ifndef _JSONP_ARGPARSER_H
#define _JSONP_ARGPARSER_H

#include <argp.h>
#include <stdbool.h>

#define JSONP_ARGS_SIZE 1

typedef struct {
  char *args[JSONP_ARGS_SIZE]; /* <JSON input file> */
  char *outfile; /* Argument for -o */
  char *infile; /* Input file with JSON records */
  JSONpEncoder encoder;
  char encoder_name[JSONP_MAX_ENCNAME_LENGTH]; /* maximum encoder name is 10 chars */
} JSONpArgs;


int jsonp_argparser(int *argc, char** argv, JSONpArgs* jsonp_args);

static error_t parse_opt (int key, char *arg, struct argp_state *state);

#endif
