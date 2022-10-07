/** @file jsonpasn1.h
 * JSONp functions and definitions
 */

#ifndef JSON_PACKER_H
#define JSON_PACKER_H

/* project version */
#define JSONP_VERSION_MAJOR 1
#define JSONP_VERSION_MINOR 0

/* no error */
#define JSONP_SUCCESS              0 /**< Success */
#define JSONP_FILE_OPEN_ERROR      1
#define JSONP_cJSON_SYNTAX_ERROR   2
#define JSONP_cJSON_PRINT_ERROR    3

#include <cJSON/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <apr_hash.h>
#include <jsonpasn1.h>
#include <apr_strings.h>

typedef enum {
    kJsonpASN1,
    kGnuASN1
} JSONpEncoder;

/**
 * @brief Reads a JSON file and generates a binary encoding
 * @param filepath The JSON file to encode
 */
int JSONp_Pack(const char *filepath);

/**
 * @brief Prints the file records (objects) from a json object
 * @param record The cJSON object
 * @return int Return code
 */
int JSONp_cJSON_print(const cJSON * record);

// Auxiliary functions

/* TLV function
   Process TLV encoding (expect Hash table and cJSON structure)
   Serialize cJSON value along with hash  to two binary files */

void JSONp_UpdateDictionary (apr_hash_t *dict, const cJSON *record, apr_pool_t *mp);
int JSONp_SerializeRecord(apr_hash_t *dict, cJSON *record, JSONpEncoder);

int JSONp_EncodeASN1(apr_hash_t *dict, const cJSON * record);

#endif // JSON_PACKER_H
