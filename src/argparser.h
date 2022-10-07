/** @file argparser.h
 * Function for command-line argument passing
 */

#ifndef __ARGPARSER__H
#define __ARGPARSER__H

#include "jsonp.h"

#define JSONP_ARGUMENT_SIZE 1

typedef struct {
  char *args[JSONP_ARGUMENT_SIZE]; /* <JSON input file> */
  char *outfile; /* Argument for -o */
  char *infile; /* Stores the input file */
} Arguments;


int argparser(int *argc, char** argv, Arguments* arguments);

static error_t
parse_opt (int key, char *arg, struct argp_state *state);

#endif
