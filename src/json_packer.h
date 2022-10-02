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

/* Main driver function */
int JSONp_Packer(const char *filepath);

// Auxiliary functions

/* TLV function
   Process TLV encoding (expect Hash table and cJSON structure)
   Serialize cJSON value along with hash  to two binary files */
void JSONp_TLVEncode(const cJSON * record);

#endif // JSON_PACKER_H
