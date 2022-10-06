//#include "asn1encoder.h"

#include <jsonpasn1.h>

int JSONp_EncodeASN1(apr_hash_t *dict, const cJSON * record) {

    /*
     * 1) Add Sequence Tag
     * 2) Iteratively add fields
     * 3) Add Sequence Lenght tag
    */

    /* Encode key-Value Pair (encrypted key plus key value)
     *
     * ASN1 Encode will be as follows:

     EncryptedkeyValue ::= SEQUENCE {
         encrkey      INTEGER
         value     UTF8String,
     }*/


    static long keys_counter = 1;

    /* Traverse json structure */

    Asn1Array encrValue, keyEncr;
    Asn1Array_Init(&encrValue);
    Asn1Array_Init(&keyEncr);


    cJSON *element = record->child;

    while (element != NULL) {

        char *key = element->string;
        long *encrypted_key = apr_hash_get (dict,  (const void*) element->string,
                                      APR_HASH_KEY_STRING);
        void *value;
        enum ASN1_Type type;

        switch (element->type & 0xFF) {
        case cJSON_Number:
            if (element->valuedouble == (int) element->valuedouble) {
                type = ASN1_TYPE_INTEGER;
                value = (void*) &element->valueint;
            }
            else {
                type = ASN1_TYPE_REAL;
                value = (void*) &element->valuedouble;
            }
            assert(type != ASN1_TYPE_REAL);
            break;
        case cJSON_True:
            type = ASN1_TYPE_BOOLEAN;
            element->valueint = 0xFF;
            value = (void*) &element->valueint;
            break;
        case cJSON_False:
            type = ASN1_TYPE_BOOLEAN;
            value = (void*) &element->valueint;
            break;
        case cJSON_String:
            type = ASN1_TYPE_UTF8_STRING;
            value = element->valuestring;
            break;
        }

        /* Encoding Records */
        Asn1Array_AppendPair(&encrValue,
                            ASN1_TYPE_INTEGER, encrypted_key, /* encrypted key */
                            type, value); /* record value */

        /* Encoding Dictionary */
        Asn1Array_AppendPair(&keyEncr,
                            ASN1_TYPE_UTF8_STRING, key, /* original key */
                            ASN1_TYPE_INTEGER, encrypted_key); /* encrypted key */

        element = element->next;

    }

    // free(derEncrkeyValuePairs);
    Asn1Array_Clear(&encrValue);
    Asn1Array_Clear(&keyEncr);

    return 0;

}

int Asn1Array_Init(Asn1Array* array) {

    enum ASN1_Class array_class;
    enum ASN1_Tag array_tag;

    array_class = ASN1_CLASS_UNIVERSAL;
    array_class |= ASN1_CLASS_STRUCTURED;
    array_tag = array_class | ASN1_TAG_SEQUENCE_OF;

    /* Tries to allocate array memory (default ASN1_ARRAY_BLOCK_SIZE bytes) */
    array->data = malloc(ASN1_ARRAY_BLOCK_SIZE);
    array->next = array->data;
    if (array->data == NULL) {
        printf("Error: couldn't allocate memory to array\n");
        return JSONp_ASN1_MEM_ERROR;
    }
    array->size = ASN1_ARRAY_BLOCK_SIZE;

    memcpy((void*) array->next++, (void*) &array_tag, 1); /* append tag identifier */
    array->next++; /* reserve 1 BYTE for length */

    return JSONp_ASN1_SUCCESS;

}


/* Appends a pair to an ASN1 array */

int Asn1Array_AppendPair(Asn1Array* array, \
                        enum ASN1_Type first_type, void* first_value, \
                        enum ASN1_Type second_type, void* second_value) {

    enum ASN1_Tag pair_class;
    enum ASN1_Tag pair_tag;
    unsigned char* pair_start; /* only pair content (ignore pair head) */

    pair_class = ASN1_CLASS_UNIVERSAL;
    pair_class |= ASN1_CLASS_STRUCTURED;
    pair_tag = ASN1_TAG_SEQUENCE | pair_class;

    assert(array->size > 2);
    /* Two bytes are needed for the tag and length fields */
    memcpy((void*) array->next++, (void*) &pair_tag, 1);
    /* Store the initial data position */
    pair_start = ++array->next;

    /* Writing first element */
    Asn1Array_Insert(array, first_type, first_value);
    /* Writing second element */
    Asn1Array_Insert(array, second_type, second_value);
    /* Setting pair length */
    *(pair_start - 1) = array->next - pair_start;

    /* Updating the array length !! */
    *(array->data + 1) += array->next - pair_start + 2;
}


int Asn1Array_Insert(Asn1Array* array, enum ASN1_Type type, void* value) {

    enum ASN1_Tag element_class;
    enum ASN1_Tag tag;

    element_class = ASN1_CLASS_UNIVERSAL;
    unsigned char length;
    size_t num_bytes = 0;

    /* Insert and update contents size */
    switch (type) {
    case ASN1_TYPE_INTEGER:
        tag = element_class | ASN1_TAG_INTEGER;
        length = 1; //sizeof(long);
        // Reducing integer here
        break;
    case ASN1_TYPE_BOOLEAN:
        tag = element_class | ASN1_TAG_BOOLEAN;
        length = 1;
        // Reducing integer here
        break;
    case ASN1_TYPE_UTF8_STRING:
        tag = element_class | ASN1_TAG_UTF8_STRING;
        length = strlen(value);
        break;
    }

    // Write tag, length and value
    num_bytes = 2 + length;
    assert(array->size - (array->next - array->data) -1 > num_bytes);
    memcpy((void*) array->next++, (void*) &tag, 1);
    memcpy((void*) array->next++, (void*) &length, 1);
    memcpy((void*) array->next, value, length);
    array->next += length;

    return 0;

}

int Asn1Array_Clear(Asn1Array* array) {
    free(array->data);
}



///******************************************************/
///* Function : _asn1_insert_tag_der                    */
///* Description: creates the DER coding of tags of one */
///* NODE.                                              */
///* Parameters:                                        */
///*   node: pointer to the tree element.               */
///*   der: string returned                             */
///*   counter: number of meanful bytes of DER          */
///*            (counter[0]..der[*counter-1]).          */
///* Return:                                            */
///*   ASN1_GENERIC_ERROR if the type is unknown,       */
///*   otherwise ASN1_SUCCESS.                          */
///******************************************************/

//int _asn1_insert_tag_der(node_asn *node, unsigned char *der, int *offset) {

//    node_asn *p;
//    int tag_len;

//    switch(type_field(node->type)){

//    case TYPE_INTEGER:
//        _asn1_tag_der(UNIVERSAL, TAG_INTEGER, der + (*offset), &tag_len);
//        break;
//    case TYPE_BOOLEAN:
//        _asn1_tag_der(UNIVERSAL, TAG_BOOLEAN, der + (*offset), &tag_len);
//        break;
//    case TYPE_OCTET_STRING:
//        _asn1_tag_der(UNIVERSAL, TAG_OCTET_STRING, der + (*offset), &tag_len);
//        break;
//    case TYPE_BIT_STRING:
//        _asn1_tag_der(UNIVERSAL, TAG_BIT_STRING, der + (*offset), &tag_len);
//        break;
//    case TYPE_SEQUENCE: case TYPE_SEQUENCE_OF:
//        _asn1_tag_der(UNIVERSAL|STRUCTURED, TAG_SEQUENCE, der + (*offset), &tag_len);
//        break;
//    case TYPE_CHOICE:
//        tag_len=0;
//        break;
//    case TYPE_ANY:
//        tag_len=0;
//        break;
//    default:
//        return ASN1_GENERIC_ERROR;
//    }

//    *offset += tag_len;

//    return ASN1_SUCCESS;
//}


///******************************************************/
///* Function : _asn1_tag_der                           */
///* Description: creates the DER coding for the CLASS  */
///* and TAG parameters.                                */
///* Parameters:                                        */
///*   class: value to convert.                         */
///*   tag_value: value to convert.                     */
///*   ans: string returned.                            */
///*   ans_len: number of meanful bytes of ANS          */
///*            (ans[0]..ans[ans_len-1]).               */
///* Return:                                            */
///******************************************************/

//void _asn1_tag_der(unsigned char class, unsigned int tag_value, unsigned char *ans, int *len_len) {
//    assert(tag_value < 30); /* Only short tag form accepted */
//    ans[0] = (class & 0xE0) + ((unsigned char)(tag_value & 0x1F));
//    *len_len = 1;
//}

//unsigned long _asn1_get_length_der(unsigned char *der,int  *len_len) {

//    assert(!(der[0]&128)); /* short lenghts (1 BYTE) only!! */
//    *len_len = 1; /* 1 BYTE */
//    return der[0]; /* Length value is in the first position after class-type
//}

