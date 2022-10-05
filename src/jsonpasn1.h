#ifndef ASN1_H
#define ASN1_H

#include <jsonp.h>
#include <assert.h>
#include <string.h>

/******************************************************/
/* Structure definition used for the node of the tree */
/* that rappresent an ASN.1 DEFINITION.               */
/******************************************************/

typedef struct node_asn_struct{
  char *name;                    /* Node name */
  unsigned int type;             /* Node type */
  unsigned char *value;          /* Node value */
  struct node_asn_struct *down;  /* Pointer to the son node */
  struct node_asn_struct *right; /* Pointer to the brother node */
  struct node_asn_struct *left;  /* Pointer to the next list element */
} node_asn;

//typedef node_asn* ASN1_TYPE;

// ASN1_SEQUENCE asn1_sequence;

// typedef struct Asn1_sequence asn1_sequence;

/*****************************************/
/*  Return codes */
/*****************************************/

#define ASN1_SUCCESS               0
#define ASN1_FILE_NOT_FOUND        1
#define ASN1_ELEMENT_NOT_FOUND     2
#define ASN1_IDENTIFIER_NOT_FOUND  3
#define ASN1_DER_ERROR             4
#define ASN1_VALUE_NOT_FOUND       5
#define ASN1_GENERIC_ERROR         6
#define ASN1_VALUE_NOT_VALID       7
#define ASN1_TAG_ERROR             8
#define ASN1_TAG_IMPLICIT          9
#define ASN1_ERROR_TYPE_ANY        10
#define ASN1_SYNTAX_ERROR          11
#define ASN1_MEM_ERROR		   12
#define ASN1_DER_OVERFLOW          13
#define ASN1_NAME_TOO_LONG         14
#define ASN1_ARRAY_ERROR           15
#define ASN1_ELEMENT_NOT_EMPTY     16

/****************************************/
/* Returns the first 8 bits.            */
/* Used with the field type of node_asn */
/****************************************/
#define type_field(x)     (x&0xFF)

/* ASN1 Classes Values */

enum ASN1_Class {

    ASN1_CLASS_UNIVERSAL = 0x00,
    ASN1_CLASS_APPLICATION = 0x40,
    ASN1_CLASS_PRIVATE = 0xC0,
    ASN1_CLASS_STRUCTURED = 0x20

};

/* ASN1 Types  */

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

//#define ASN1_TYPE_INTEGER        3
//#define ASN1_TYPE_BOOLEAN        4
//#define ASN1_TYPE_SEQUENCE       5
//#define ASN1_TYPE_BIT_STRING     6
//#define ASN1_TYPE_OCTET_STRING   7
//#define ASN1_TYPE_SEQUENCE_OF   11
//#define ASN1_TYPE_OBJECT_ID     12
//#define ASN1_TYPE_ANY           13
//#define ASN1_TYPE_SET           14
//#define ASN1_TYPE_SET_OF        15
//#define ASN1_TYPE_DEFINITIONS   16
//#define ASN1_TYPE_REAL          17

//#define ASN1_TYPE_GENERALSTRING 27
//#define ASN1_TYPE_NUMERIC_STRING 28
//#define ASN1_TYPE_IA5_STRING     29
//#define ASN1_TYPE_PRINTABLE_STRING 31
//#define ASN1_TYPE_UTF8_STRING    34
//#define ASN1_TYPE_VISIBLE_STRING 35

/* ASN1 Universal Tag values */

enum ASN1_Tag {
    ASN1_TAG_BOOLEAN = 0x01,
    ASN1_TAG_INTEGER = 0x02,
    ASN1_TAG_SEQUENCE =	0x10,
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

#define ASN1_SEQUENCE_BLOCK_SIZE 4096

typedef struct {

//    enum ASN1_Class asn1_class; /* Container class according to ASN1 */
//    enum ASN1_Tag asn1_tag; /* Container tag according to ASN1 */
    unsigned char *data; /* Der encoding of the container */
    unsigned char *end; /* points to next available space in data */
    size_t content_size; /* size in bytes */

} Asn1Sequence;

int Asn1Sequence_Init(Asn1Sequence* sequence);
int Asn1Sequence_Clear(Asn1Sequence* sequence);

int Asn1Sequence_Insert(Asn1Sequence* , enum ASN1_Type type, void* value);

int JSONp_ASN1Encode(apr_hash_t *dict, const cJSON * record);

#endif // ASN1_H
