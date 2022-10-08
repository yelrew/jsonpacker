/** @file argparser.h
 * Function for command-line argument passing
 */

#ifndef _JSONP_ARGPARSER_H
#define _JSONP_ARGPARSER_H

#include <argp.h>
#include <stdbool.h>

#define JSONP_ARGS_SIZE 1

/**
 * A strucutre to store JSONp command-line arguments
 */
typedef struct {
  char *args[JSONP_ARGS_SIZE+5]; /* <JSON input file> */
  char *outfile; /**< Argument for -o */
  char *infile; /**< Input file with JSON records */
  bool one_dict; /**< Use a single dictionary for all records */
  bool print_encodings; /**< Print binary encodings */
  bool print_records; /**< Print records in compacted JSON style */
  bool print_records_full ; /**< Print records in a full JSON style */
  JSONpEncoder encoder; /**< Encoder type */
  char encoder_name[JSONP_MAX_ENCNAME_LENGTH]; /**< maximum encoder name length */
} JSONpArgs;

/**
 * @brief Parse the command-line arguments *
 * @param argc Arguments count
 * @param argv Arguments' string array
 * @param jsonp_args JSONp structure that controls the program options
 * @return void
 */
void jsonp_argparser(int *argc, char** argv, JSONpArgs* jsonp_args);

/**
 *  Argp external parser
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state);

#endif
