/** @file jsonpasn1.h
 * JSONp functions and definitions
 */

#ifndef JSON_PACKER_H
#define JSON_PACKER_H

/**
 * JSONpEncoder.
 * Encoder types handled by JSONp
 */
typedef enum {
    JpASN1, /**< Default internal ASN1 encoder */
    GnuASN1, /**< GNU Libtasn */
    libBSON, /**< LibBSON */
} JSONpEncoder;

#define JSONP_MAX_ENCNAME_LENGTH 10 /**< maximum encoder name length !! */

/* External headers */
#include <stdio.h>
#include <stdlib.h>
#include <apr_hash.h>
#include <apr_strings.h>
#include <argp.h>
#include <argparser.h>
#include <cJSON/cJSON.h>

/* JSONp headers */
#include <asn1.h>

/* project version */
#define JSONP_VERSION_MAJOR 1
#define JSONP_VERSION_MINOR 0

/* Exit codes */
#define JSONP_SUCCESS              0 /**< Success! */
#define JSONP_FILE_OPEN_ERROR      1 /**< Probllem when opening file */
#define JSONP_MISSING_ENCODER      2 /**< Encoder not implemented */
#define JSONP_cJSON_SYNTAX_ERROR   3
#define JSONP_cJSON_PRINT_ERROR    4
#define JSONP_cJSON_INVALID_TYPE   5 /**< JSON object not handled */
#define JSONP_APR_MISSING_KEY      6


#define _TO_STRING_(_NAME_) #_NAME_

/**
 * @brief Reads a JSON file and generates a binary encoding
 * @param filepath The JSON file to encode
 */
int JSONp_Pack(JSONpArgs *jsonp_args);

/**
 * @brief Prints the JSON object which holds a single record
 * @param record The cJSON object
 * @return int exit code
 */
int JSONp_cJSON_print(const cJSON * record);

// Auxiliary functions

/* TLV function
   Process TLV encoding (expect Hash table and cJSON structure)
   Serialize cJSON value along with hash  to two binary files */

/**
 * @brief Prints the record encoding (encoded record and dictionary)
 * @param record The cJSON object containing a record
 * @param dict The ARP dictionary with the key-value (string-long) pairs
 * @return int exit code
 */
int JSONp_PrintEncoding(apr_hash_t *dict, const cJSON *record);

/**
 * @brief Prints the full dict
 * @param dict The ARP dictionary with the key-value (string-long) pairs
 * @return int exit code
 */
int JSONp_PrintDict(apr_hash_t *dict);

void JSONp_UpdateDictionary (apr_hash_t *dict, const cJSON *record, apr_pool_t *mp);

int JSONp_SerializeRecord(apr_hash_t *dict, cJSON *record, JSONpArgs* jsonp_args);

int JSONp_EncodeASN1(apr_hash_t *dict, const cJSON * record);

#endif // JSON_PACKER_H
