//#include "asn1encoder.h"

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

