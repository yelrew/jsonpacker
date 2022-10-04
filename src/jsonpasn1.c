//#include "asn1encoder.h"

#include <jsonpasn1.h>

int JSONp_ASN1Serialize(apr_hash_t *dict, const cJSON * record) {

    /*
     * 1) Add Sequence Tag
     * 2) Iteratively add fields
     * 3) Add Sequence Lenght tag
    */

    unsigned char *derEncrkeyValuePairs = NULL, *derKeyEncrkeyPairs = NULL;
    unsigned char sequence_class, sequence_tag;

    /* Encode key-Value Pair (encrypted key plus key value)
     *
     * ASN1 Encode will be as follows:

     EncryptedkeyValue ::= SEQUENCE {
         encrkey      INTEGER
         value     UTF8String,
     }*/

    sequence_class = ASN1_CLASS_UNIVERSAL;
    sequence_class |= ASN1_CLASS_STRUCTURED;
    sequence_tag = ASN1_TAG_SEQUENCE;

    static long keys_counter = 1;

    /* Traverse json structure */

    cJSON *element = record->child;

    while (element != NULL) {

        char *key = element->string;
        char *encrKey = apr_hash_get (dict,  (const void*) element->string,
                                      APR_HASH_KEY_STRING);
        void *value;
        int type;

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
            assert(value_type != ASN1_TYPE_REAL);
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

        /* JSONp_EncodeASN1Element(type, void* value);
         *
         * // Records
         *
         * JSONp_EncodeASN1Element(ASN1_TYPE_INTEGER, encrKey, derEncrkeyValuePairs);
         * JSONp_EncodeASN1Element(type, value, derEncrkeyValuePairs);
         *
         * // Dictionary
         *
         * JSONp_EncodeASN1Element(ASN1_TYPE_UTF8_STRING, key, derKeyEncrkeyPairs);
         * JSONp_EncodeASN1Element(ASN1_TYPE_INTEGER, encrKey, derKeyEncrkeyPairs);

        */


        element = element->next;
    }

    free(derEncrkeyValuePairs);
    free(derKeyEncrkeyPairs);

    return 0;

    /* Checking Dictionary */
//    apr_hash_index_t *hi;
//    for (hi = apr_hash_first(NULL, ht); hi; hi = apr_hash_next(hi)) {
//        const char *k;
//        const char *v;
//        apr_hash_this(hi, (const void**) &k, NULL, (void**) &v);
//        printf("ht iteration: key=%s, val=%s\n", k, v);
//    }

//    if (tag_value < 31)
//      {
//        /* short form */
//        ans[0] = (class & 0xE0) + ((unsigned char) (tag_value & 0x1F));
//        *ans_len = 1;
//      }




    /* Encode Dictionary
     *
     * ASN1 Encode will be as follows:

     KeyEncryption ::= SEQUENCE {
         key      UTF8String
         enc_key     INTEGER,
     }

     */

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

