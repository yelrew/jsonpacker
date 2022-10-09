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

/**
 * @defgroup JSONp_Definitions JSONp Definitions
 *
 * @{
 */

/* project version */
#define JSONP_VERSION_MAJOR 1
#define JSONP_VERSION_MINOR 0

/* Exit codes */
#define JSONP_SUCCESS              0 /**< Success! */
#define JSONP_FILE_OPEN_ERROR      1 /**< Problem when opening file */
#define JSONP_MISSING_ENCODER      2 /**< Encoder not implemented */
#define JSONP_cJSON_SYNTAX_ERROR   3
#define JSONP_cJSON_PRINT_ERROR    4
#define JSONP_cJSON_INVALID_TYPE   5 /**< JSON object not handled */
#define JSONP_APR_MISSING_KEY      6

#define JSONP_INT_SIZE 4
#define MAX_LONG_INT_DIGITS 50
#define _TO_STRING_(_NAME_) #_NAME_

/** @} */

/**
 * @brief Reads a text file and extracts JSON records for packing
 * @param jsonp_args Pointer to the command-line arguments
 * @return int Exit code
 */
int JSONp_Pack(JSONpArgs *jsonp_args);

/**
 * @brief Process a JSON record and assigns a numeric key
 * @param record cJSON object containing a record
 * @param dict ARP dictionary to store key-value (string-int) pairs
 * @param jsonp_args Pointer to the command-line arguments
 * @param mp Pointer to an Apache Runtime memory pool
 * @return int Exit code
 */
int JSONp_ProcessRecord(cJSON *record, apr_hash_t *dict,
                        JSONpArgs* jsonp_args, apr_pool_t *mp);

/**
 * @brief Encodes a JSON record and the corresponding numeric keys in binary format
 *
 *        For a given JSON record, splits each key-value pair into an `encValue` pair,
 *        where the keys are replaced by `numeric` pre-assigned values, and a `keyEnc`
 *        pair, with a mapping between the original keys and the new encoded keys.
 *
 * @param dict  Dictionary (hash table) where the entries are the keys
 *              and the values are the encrypted keys
 * @param record JSON object (key value pairs)
 * @param jsonp_args JSONp command-line arguments
 *
 * @param encValue Pointer to an Asn1Array containing the resulting
 *        `numeric key` : `value` binary encoding
 * @param keyEnc Pointer to an Asn1Array containing the resulting
 *        `key` : `numeric key` binary encoding
 *
 * @return int return status
*/
int JSONp_EncodeRecord(apr_hash_t *dict, cJSON *record, JSONpArgs* jsonp_args,
                       Asn1Array *encValue, Asn1Array *keyEnc);

/**
 * @brief Add the keys of a JSON record to a hash-table
 * @param dict ARP dictionary to store key-value (string-int) pairs
 * @param record cJSON object containing a record
 * @param jsonp_args Pointer to the command-line arguments
 * @param mp Pointer to an Apache Runtime memory pool
 * @remark Existing keys are not overwritten.
 * @return int Exit code
 */
void JSONp_UpdateDict (apr_hash_t *dict, const cJSON *record,
                       JSONpArgs* jsonp_args, apr_pool_t *mp);

/**
 * @brief Prints the full dict
 * @param dict The ARP dictionary with the key-value (string-long) pairs
 * @return int exit code
 */
int JSONp_PrintDict(apr_hash_t *dict);

/**
 * @brief Prints the JSON object which holds a single record
 * @param record cJSON object containing the record
 * @param jsonp_args Pointer to the program's options structure
 * @return int exit code
 */
int JSONp_cJSON_print(const cJSON * record, JSONpArgs *jsonp_args);

/**
 * @brief Prints the record encoding (encoded record and dictionary)
 * @param record The cJSON object containing a record
 * @param dict The ARP dictionary with key-value (string-int) pairs
 * @param jsonp_args Pointer to the program's options structure
 * @return int exit code
 */
int JSONp_PrintEncoding(apr_hash_t *dict, const cJSON *record, JSONpArgs *jsonp_args);


#endif // JSON_PACKER_H
