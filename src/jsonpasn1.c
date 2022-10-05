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

    Asn1Sequence encrValue, keyEncr;
    Asn1Sequence_Init(&encrValue);
    Asn1Sequence_Init(&keyEncr);


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
        Asn1Sequence_Insert(&encrValue, ASN1_TYPE_INTEGER, encrypted_key);
        Asn1Sequence_Insert(&encrValue, type, value);

        /* Encoding Dictionary */
        Asn1Sequence_Insert(&keyEncr, ASN1_TYPE_UTF8_STRING, key);
        Asn1Sequence_Insert(&keyEncr, ASN1_TYPE_INTEGER, encrypted_key);

        element = element->next;
    }

    // free(derEncrkeyValuePairs);
    Asn1Sequence_Clear(&encrValue);
    Asn1Sequence_Clear(&keyEncr);

    return 0;

}

int Asn1Sequence_Init(Asn1Sequence* sequence) {

    unsigned char class, tag;

    class = ASN1_CLASS_UNIVERSAL;
    class |= ASN1_CLASS_STRUCTURED;
    tag = ASN1_TAG_SEQUENCE;
    sequence->content_size = 0;

    sequence->data = malloc(ASN1_SEQUENCE_BLOCK_SIZE);
    memcpy((void*) sequence->data, (void*) &class, 1); /* append class identifier */
    memcpy((void*) sequence->data, (void*) &tag, 1); /* append tag identifier */
    sequence->end = sequence->data + 3; /* reserve 1 BYTE for length */
}

int Asn1Sequence_Clear(Asn1Sequence* sequence) {
    free(sequence->data);
}

int Asn1Sequence_Insert(Asn1Sequence* sequence, enum ASN1_Type type, void* value) {

}

int Asn1Sequence_Insert(Asn1Sequence* sequence, enum ASN1_Type type, void* value) {

    enum ASN1_Tag class;
    enum ASN1_Tag tag;
    class = ASN1_CLASS_UNIVERSAL;
    class |= ASN1_CLASS_STRUCTURED;
    size_t value_length;

    /* Insert and update contents size */
    switch (type) {
    case ASN1_TYPE_INTEGER:
        tag = ASN1_TAG_INTEGER;
        value_length = sizeof(long);
        // Reducing integer here
        break;
    case ASN1_TYPE_BOOLEAN:
        tag = ASN1_TAG_BOOLEAN;
        value_length = 1;
        // Reducing integer here
        break;
    case ASN1_TYPE_UTF8_STRING:
        tag = ASN1_TAG_UTF8_STRING;
        value_length = sizeof(long);
        break;
    }


    // sequence->content_size += 0;
    // assert(sequence->content_size < 128); /* short-length length only */
    //sequence->data + 2 = length;


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

