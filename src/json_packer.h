/* JSON Packer C */
/* A JSON TLV packer in C. */

#ifndef JSON_PACKER_H
#define JSON_PACKER_H

/* project version */
#define JSON_PACKER_VERSION_MAJOR 1
#define JSON_PACKER_VERSION_MINOR 0

#include <cJSON/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Main driver function */
int JSONp_Packer(const char *filepath);

// Auxiliary functions

/* Main driver function */
void JSONp_TLVEncode();

#endif // JSON_PACKER_H
