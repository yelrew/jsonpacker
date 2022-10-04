/* JSON Packer C */
/* A JSON TLV packer in C. */

#ifndef JSON_PACKER_H
#define JSON_PACKER_H

/* project version */
#define JSON_PACKER_VERSION_MAJOR 1
#define JSON_PACKER_VERSION_MINOR 0

/* no error */
#define JSONP_SUCCESS 0

#include <cJSON/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <apr_hash.h>
//#include <apr_strings.h>

typedef enum {
    kJsonpASN1,
    kGnuASN1
} JSONpEncoder;

/* Main driver function */
int JSONp_Pack(const char *filepath);

// Auxiliary functions

/* TLV function
   Process TLV encoding (expect Hash table and cJSON structure)
   Serialize cJSON value along with hash  to two binary files */

int JSONp_UpdateDictionary (apr_hash_t *dict, cJSON *record, apr_pool_t *mp);
int JSONp_SerializeRecord(apr_hash_t *dict, cJSON *record, JSONpEncoder);

int JSONp_EncodeASN1(apr_hash_t *dict, const cJSON * record);

#endif // JSON_PACKER_H
