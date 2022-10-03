#ifndef ASN1_H
#define ASN1_H

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

typedef node_asn* ASN1_TYPE;

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


/* List of constants for field type of typedef node_asn  */
#define TYPE_CONSTANT     1
#define TYPE_IDENTIFIER   2
#define TYPE_INTEGER      3
#define TYPE_BOOLEAN      4
#define TYPE_SEQUENCE     5
#define TYPE_BIT_STRING   6
#define TYPE_OCTET_STRING 7
#define TYPE_TAG          8
#define TYPE_DEFAULT      9
#define TYPE_SIZE        10
#define TYPE_SEQUENCE_OF 11
#define TYPE_OBJECT_ID   12
#define TYPE_ANY         13
#define TYPE_SET         14
#define TYPE_SET_OF      15
#define TYPE_DEFINITIONS 16
#define TYPE_TIME        17
#define TYPE_CHOICE      18
#define TYPE_IMPORTS     19
#define TYPE_NULL        20
#define TYPE_ENUMERATED  21

#define UNIVERSAL        0x00
#define APPLICATION      0x40
#define CONTEXT_SPECIFIC 0x80
#define PRIVATE          0xC0
#define STRUCTURED       0x20

/* ASN1 Universal Tag values */

#define UNIVERSAL        0x00
#define APPLICATION      0x40
#define CONTEXT_SPECIFIC 0x80
#define PRIVATE          0xC0
#define STRUCTURED       0x20

#define TAG_BOOLEAN          0x01
#define TAG_INTEGER          0x02
#define TAG_SEQUENCE         0x10
#define TAG_SET              0x11
#define TAG_OCTET_STRING     0x04
#define TAG_BIT_STRING       0x03
#define TAG_UTCTime          0x17
#define TAG_GENERALIZEDTime  0x18
#define TAG_OBJECT_ID        0x06
#define TAG_ENUMERATED       0x0A
#define TAG_NULL             0x05

#endif // ASN1_H
