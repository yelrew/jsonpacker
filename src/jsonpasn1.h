/** @file jsonpasn1.h
 * Functions and definitions for the JSONp-ASN1 interface
 */

#ifndef ASN1_H
#define ASN1_H
#include <jsonp.h>
#include <assert.h>
#include <string.h>

/**
 * @ingroup JSONp ASN1 Return codes
 *
 * @{
 */

#define JSONP_ASN1_SUCCESS           0 /**< ASN1 Encoding sucess */
#define JSONP_ASN1_MEM_ERROR         1
//#define ASN1_ELEMENT_NOT_FOUND     2
//#define ASN1_IDENTIFIER_NOT_FOUND  3
//#define ASN1_DER_ERROR             4
//#define ASN1_VALUE_NOT_FOUND       5
//#define ASN1_GENERIC_ERROR         6
//#define ASN1_VALUE_NOT_VALID       7
//#define ASN1_TAG_ERROR             8
//#define ASN1_TAG_IMPLICIT          9
//#define ASN1_ERROR_TYPE_ANY        10
//#define ASN1_SYNTAX_ERROR          11
//#define ASN1_MEM_ERROR             12
//#define ASN1_DER_OVERFLOW          13
//#define ASN1_NAME_TOO_LONG         14
//#define ASN1_ARRAY_ERROR           15
//#define ASN1_ELEMENT_NOT_EMPTY     16

#define ASN1_ARRAY_BLOCK_SIZE 4096

/** @} */

/**
 * ASN1_Class.
 * Default ASN1 classes values
 */
enum ASN1_Class {
    ASN1_CLASS_UNIVERSAL = 0x00,
    ASN1_CLASS_APPLICATION = 0x40,
    ASN1_CLASS_PRIVATE = 0xC0,
    ASN1_CLASS_STRUCTURED = 0x20
};

/**
 * ASN1_Type.
 * Default ASN1 types values
 */
enum ASN1_Type {
    ASN1_TYPE_INTEGER,
    ASN1_TYPE_BOOLEAN,
    ASN1_TYPE_SEQUENCE,
    ASN1_TYPE_BIT_STRING,
    ASN1_TYPE_OCTET_STRING,
    ASN1_TYPE_SEQUENCE_OF,
    ASN1_TYPE_OBJECT_ID,
    ASN1_TYPE_ANY,
    ASN1_TYPE_SET,
    ASN1_TYPE_SET_OF,
    ASN1_TYPE_DEFINITIONS,
    ASN1_TYPE_REAL,
    ASN1_TYPE_GENERALSTRING,
    ASN1_TYPE_NUMERIC_STRING,
    ASN1_TYPE_IA5_STRING,
    ASN1_TYPE_PRINTABLE_STRING,
    ASN1_TYPE_UTF8_STRING,
    ASN1_TYPE_VISIBLE_STRING,
};

/**
 * ASN1_Tag.
 * Default ASN1 tag values
 */
enum ASN1_Tag {
    ASN1_TAG_BOOLEAN = 0x01,
    ASN1_TAG_INTEGER = 0x02,
    ASN1_TAG_SEQUENCE =	0x10,
    ASN1_TAG_SEQUENCE_OF = 0x10,
    ASN1_TAG_SET = 0x11,
    ASN1_TAG_OCTET_STRING = 0x04,
    ASN1_TAG_BIT_STRING = 0x03,
    ASN1_TAG_NULL = 0x05,
    ASN1_TAG_GENERALSTRING = 0x1B,
    ASN1_TAG_NUMERIC_STRING = 0x12,
    ASN1_TAG_IA5_STRING = 0x16,
    ASN1_TAG_PRINTABLE_STRING =	0x13,
    ASN1_TAG_UTF8_STRING = 0x0C,
    ASN1_TAG_VISIBLE_STRING = 0x1A,
};

/**
 * Asn1Array_AN1Encode
 * Encodes a JSON record and its dictionarys keys in ASN.1 der binary format
 *
 * @param record JSON object (key value pairs)
 * @param dict  Dictionary (hash table) where the entries are the keys
 *              and the values are the encrypted keys
 * @return int return status
 * @remarks
 *     Encode key-Value Pair (encrypted key plus key value)
     *
     * ASN1 Encode will be as follows:

     EncryptedkeyValue ::= SEQUENCE {
         encrkey      INTEGER
         value     UTF8String,
     }

*/
int JSONp_ASN1Encode(const cJSON * record, const apr_hash_t *dict);


/**
 * @struct Asn1Array
 * @brief An array structure representing the `SEQUENCE OF` ASN1 tag.
 *
 * Asn1Array stores the contents of a `SEQUENCE OF` tag as a char array.
 * The buffer size and the next available position are also stored.
 */
typedef struct {
    unsigned char *data; /*!< Der encoding of the container */
    unsigned char *next; /*!< points to next available space in data */
    size_t size; /*!< array size in bytes */
} Asn1Array;

/**
 * Asn1Array_Init
 * Initializes the struct Asn1Array
 *
 * @param array Asn1Array array
 * @return status Initialization results
 */
int Asn1Array_Init(Asn1Array* array);

/**
 * Asn1Array_Clear
 * Clear the struct Asn1Array
 *
 * @param array Asn1Array array
 * @return int cleanup results
 */
int Asn1Array_Clear(Asn1Array* array);

/**
 * Asn1Array_Insert
 * Insert a simple ASN.1 element into an Asn1Array
 *
 * @param array Asn1Array array
 * @param type element's type
 * @param value element's value
 * @return int return status
 */
int Asn1Array_Insert(Asn1Array* array, enum ASN1_Type type, void* value);

/**
 * Asn1Array_AppendPair
 * Appends a pair of ASN.1 elements into an existing Asn1Array
 *
 * @param array Asn1Array array to append
 * @param first_type First element's type
 * @param first_value First element's value
 * @param second_type Second element's type
 * @param second_value Second element's value*
 * @return status return status
 *
 * @note A pair is modeled as an `SEQUENCE` ASN1 tag with two elements.
 */
int Asn1Array_AppendPair(Asn1Array* array, \
                        enum ASN1_Type first_type, void* first_value, \
                        enum ASN1_Type second_type, void* second_value);

#endif
