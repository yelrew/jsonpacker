//#include "asn1encoder.h"

#include <asn1.h>

int JSONp_ASN1EncodeRecord(const cJSON * record, apr_hash_t *dict, JSONpArgs* jsonp_args,
                           Asn1Array *encValue, Asn1Array *keyEnc) {

    /* Traverse json structure */
    cJSON *element = record->child;

    while (element != NULL) {

        char *key = element->string;
        long *encrypted_key;
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
        default:
            return JSONP_cJSON_INVALID_TYPE;
        }

        encrypted_key = apr_hash_get (dict,  (const void*) element->string,
                                      APR_HASH_KEY_STRING);
        /* Encoding Records */
        Asn1Array_AppendPair(encValue,
                             ASN1_TYPE_INTEGER, encrypted_key, /* encrypted key */
                             type, value); /* record value */

        /* Encoding Dictionary */
        Asn1Array_AppendPair(keyEnc,
                             ASN1_TYPE_UTF8_STRING, key, /* original key */
                             ASN1_TYPE_INTEGER, encrypted_key); /* encrypted key */

        element = element->next;

    }

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
        /* Number of significant bytes of the integer */
        length = MinSignedIntLength(*((int*) value));
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

    if (type != ASN1_TYPE_INTEGER)
        memcpy((void*) array->next, value, length);
    else {
        /* Write big endian */
        unsigned char* byte = value;
        unsigned char* pt = array->next + length - 1;
        while (pt != array->next - 1) {
            memcpy((void*) pt--, byte++, 1);
        }
    }
    array->next += length;
    return 0;

}

unsigned char MinSignedIntLength (int num) {

    int sign_bit, first_oct_msk, second_oct_b8_msk, nbytes;
    int szint = JSONP_INT_SIZE;

    sign_bit = ((num & (0x1 << (szint*8-1)) ) >> (szint*8-1)) & 1; /* 64 bit? */
    first_oct_msk = 0xff << ((szint-1)*8); /* 0xff000000 */
    second_oct_b8_msk = 0x80 << ((szint-2)*8); /* 0x00800000 */
    nbytes = szint;

    while (nbytes > 1 && ((num & first_oct_msk) == first_oct_msk*sign_bit) && \
           ((num & second_oct_b8_msk) == second_oct_b8_msk*sign_bit))
    {
        first_oct_msk >>= 8;
        second_oct_b8_msk >>= 8;
        nbytes -= 1;
    }
    return nbytes;
}

int Asn1Array_Print(Asn1Array* array, char* message) {

    fprintf(stdout, "%s", message);
    int num_bytes_encoded = array->next - array->data;
    unsigned char* array_element = array->data;
    /* Print DER encoding */
    fprintf (stdout, " ----------------------\n Number of bytes=%d\n ", num_bytes_encoded);
    while (array_element != array->next)
      fprintf (stdout, "%02X ", *array_element++);
    fputs ("\n ----------------------\n\n", stdout);

}

int Asn1Array_Init(Asn1Array* array, unsigned char* name,
                                     unsigned char* suffix) {

    enum ASN1_Class array_class;
    enum ASN1_Tag array_tag;
    size_t strlen_suffix;

    /* Check if the name suffix was provided */
    if (suffix == NULL)
        strlen_suffix = 0;
    else
        strlen_suffix = strlen(suffix);

    array_class = ASN1_CLASS_UNIVERSAL;
    array_class |= ASN1_CLASS_STRUCTURED;
    array_tag = array_class | ASN1_TAG_SEQUENCE_OF;


    array->name = malloc( strlen(name) + strlen("-") + strlen_suffix + 1);
    assert (array->name != NULL);
    array->name[0] = '\0';
    strcat(array->name, name);
    strcat(array->name, "-");
    if (strlen_suffix)
        strcat(array->name, suffix);

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

int Asn1Array_WriteToFile (Asn1Array* array, JSONpArgs* jsonp_args) {

    FILE *outputFile;
    static const char *outfile_extension = "der";

    // TODO: Test outputFileName == NULL (command line default)
    jsonp_args->outfile = malloc(strlen (jsonp_args->infile) + \
                                 strlen ("-") + \
                                 strlen (array->name) + \
                                 strlen (".") + \
                                 strlen(outfile_extension) + 1);

    if (jsonp_args->outfile == NULL) {
        printf("Error: couldn't allocate memory\n");
        return JSONP_ASN1_MEM_ERROR;
    }
    strcpy (jsonp_args->outfile, jsonp_args->infile);
    if (strrchr(jsonp_args->outfile, '.') != NULL)
        *strrchr (jsonp_args->outfile, '.') = '\0';

    strcat(jsonp_args->outfile, "-");
    strcat(jsonp_args->outfile, array->name);
    strcat(jsonp_args->outfile, ".");
    strcat(jsonp_args->outfile, outfile_extension);

    /* send to log what I am doing ! -> printf (stderr, "\nOutputFile=%s\n", outputFileName); */

    outputFile = fopen (jsonp_args->outfile, "w");
    if (outputFile == NULL) {
        fprintf (stderr,
                 "Asn1Array_WriteToFile: can\'t write to file '%s'\n",
                 jsonp_args->outfile);
        return JSONP_ASN1_FILE_WRITE_ERROR;
    }

    unsigned char* array_element = array->data;

    /* Print ASN.1 DER encoding */
    while (array_element != array->next)
        fprintf (outputFile, "%02X ", *array_element++);

    fclose (outputFile);
    fprintf(stdout, "Sucessfully wrote file \"%s\".\n\n", jsonp_args->outfile);
    free (jsonp_args->outfile);

    return JSONP_ASN1_SUCCESS;

}

int Asn1Array_Clear(Asn1Array* array) {

    free(array->data);
    free(array->name);
}
