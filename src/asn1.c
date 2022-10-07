//#include "asn1encoder.h"

#include <asn1.h>

int JSONp_ASN1Encode(const cJSON * record, apr_hash_t *dict) {

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
            } else {
                type = ASN1_TYPE_REAL;
                value = (void*) &element->valuedouble;
                fprintf (stderr, "Error: Invalid JSON type detected!\n");
                return JSONP_cJSON_INVALID_TYPE;
            }            
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
    /* Print binary array */
    Asn1Array_Print(&encrValue, "ASN.1 (DER) encoded records\n");
    Asn1Array_Print(&keyEncr, "ASN.1 (DER) encoded keys\n");

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
        return JSONP_ASN1_MEM_ERROR;
    }
    array->size = ASN1_ARRAY_BLOCK_SIZE;

    memcpy((void*) array->next++, (void*) &array_tag, 1); /* append tag identifier */
    array->next++; /* reserve 1 BYTE for length */

    return JSONP_ASN1_SUCCESS;

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


int Asn1Array_Print(Asn1Array* array, char* message) {

    fprintf(stdout, "%s\n", message);
    int num_bytes_encoded = array->next - array->data;
    unsigned char* array_element = array->data;
    /* Print DER encoding */
    fprintf (stdout, "/***************/\nNumber of bytes=%d\n", num_bytes_encoded);
    while (array_element != array->next)
      fprintf (stdout, "%02X ", *array_element++);
    fputs ("\n/***************/\n\n", stdout);

    fprintf(stdout, "");


}
