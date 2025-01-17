/**
 * @file jsonp.c
 * @brief JSONp functions and definitions
 */

#include <jsonp.h>
#include <argparser.h>

/**
 * @addtogroup JSONp
 *
 * @{
 */

int JSONp_Pack(JSONpArgs* jsonp_args) {

    const char *records_filepath = jsonp_args->infile;
    int status = 0;
    apr_pool_t *mp; /* apr memory pool */
    apr_hash_t *dict; /* dictionary */

    fprintf(stdout, "JSON Packer %d.%d\n\n", \
            JSONP_VERSION_MAJOR, JSONP_VERSION_MINOR);

    /* Open file containing JSON records */
    FILE *fp = fopen(records_filepath, "r");
    if(fp == NULL) {
        fprintf(stderr, "Unable to open file \"%s\"\n", records_filepath);
        return JSONP_FILE_OPEN_ERROR;
    }

    /* Creating APR memory pool */
    apr_initialize();
    apr_pool_create(&mp, NULL);
    dict = apr_hash_make(mp);

    /* Parse records */

    char *line = NULL;
    size_t len = 0;

    while(getline(&line, &len, fp) != -1) {

        cJSON *record = cJSON_Parse(line);
         /* Processing current record */
        status = JSONp_ProcessRecord(record, dict, jsonp_args, mp);

        cJSON_Delete(record);
        if (status != JSONP_SUCCESS) break;
    }

    /* The hash table is automatically destroyed after @mp */
    apr_pool_destroy(mp);
    apr_terminate();
    fclose(fp);

    return status;
}


int JSONp_ProcessRecord(cJSON *record, apr_hash_t *dict,
                        JSONpArgs* jsonp_args, apr_pool_t *mp) {
    int status = 0;
    static size_t record_num = 0;

    if (record == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON Syntax Error before: %s\n", error_ptr);
        }
        cJSON_Delete(record);
        return JSONP_cJSON_SYNTAX_ERROR;
    }

    /* Set top-level record string (will go deallocated after cJSON_Delete) */
    assert ((record->string = malloc(MAX_LONG_INT_DIGITS)) != NULL);
    sprintf(record->string, "%d", ++record_num);

    /* Print tasks */
    fprintf(stdout, "/**********************************\n\n");
    fprintf(stdout, "Processing record %d ...\n\n", record_num);
    status = JSONp_cJSON_print(record, jsonp_args);
    if (status!= JSONP_SUCCESS)
        return status;

    /* Add current keys to dictionary */
    JSONp_UpdateDict(dict, record, jsonp_args, mp);

    /* Print numeric keys/values and keys/numeric keys */
    status = JSONp_PrintEncoding(dict, record, jsonp_args);
    if (status!= JSONP_SUCCESS)
        return status;

    /* Generate Binary Encoding */
    status = JSONp_EncodeRecord(dict, record, jsonp_args, NULL, NULL);

    return status;
}


int JSONp_EncodeRecord(apr_hash_t *dict, cJSON *record, JSONpArgs* jsonp_args,
                       Asn1Array *encValue, Asn1Array *keyEnc) {

    int ret_status = 0;

    /* Initializing Asn1Arrays */

    bool free_encValue = false;
    bool free_keyEnc = false;
    if (encValue == NULL) {
        encValue = malloc (sizeof(Asn1Array));
        Asn1Array_Init(encValue, "values", record->string);
        free_encValue = true;
    }
    if (keyEnc == NULL) {
        keyEnc = malloc (sizeof(Asn1Array));
        Asn1Array_Init(keyEnc, "keys", record->string);
        free_keyEnc = true;
    }

    /* Calling binary encoder */
    switch(jsonp_args->encoder) {
    case JpASN1:
        ret_status = JSONp_ASN1EncodeRecord(record, dict, jsonp_args, encValue, keyEnc);
        break;
    default:
        fprintf(stderr, "Error: unknow encoder type \"%s\"\n", jsonp_args->encoder_name);
        ret_status = JSONP_INVALID_ENCODER;
        break;
    }

    if (ret_status == JSONP_ASN1_SUCCESS) {
        /* Print binary array */
        if (jsonp_args->print_encodings) {
            Asn1Array_Print(encValue, " Encoded records in ASN.1 DER:\n");
            Asn1Array_Print(keyEnc, " Encoded keys in ASN.1 DER:\n");
        }
        /* Write to file */
        if (jsonp_args->write_binary_files) {
            Asn1Array_WriteToFile(encValue, jsonp_args);
            Asn1Array_WriteToFile(keyEnc, jsonp_args);
        }
    }

    if (free_keyEnc) {
        Asn1Array_Clear(keyEnc);
        free(keyEnc);
    }
    if (free_encValue) {
        Asn1Array_Clear(encValue);
        free(encValue);
    }

    return ret_status;
}


int JSONp_PrintEncoding(apr_hash_t *dict, const cJSON *record, JSONpArgs* jsonp_args) {

    if (!jsonp_args->print_records && !jsonp_args->print_records_full)
        return JSONP_SUCCESS;

    cJSON *pair = record->child;

    /* Print encoded values (Numeric key : value) */

    fprintf(stdout, " Encoded values:\n\n {");
    if (jsonp_args->print_records_full)
        fprintf(stdout, "\n");

    while (pair != NULL) {
        const unsigned char* key;
        const long *enc_key;
        const char *value;

        key = pair->string;
        enc_key = apr_hash_get (dict,  (const void*) key, APR_HASH_KEY_STRING);

        if (enc_key == NULL) {
            fprintf (stderr, "Error when printing dicionary\n.");
            fprintf(stderr, "The key \"%s\" does not exist.\n",  key);
            return JSONP_APR_MISSING_KEY;
        }

        value =  cJSON_Print(pair);
        if (jsonp_args->print_records_full)
            fprintf(stdout, "\t");
        fprintf(stdout, " %ld : %s", *enc_key, value); /* encoded key (number) : value */

        if (pair->next) {
            fprintf(stdout, ",");
            if (jsonp_args->print_records_full)
                fprintf(stdout, "\n");
        }
        pair = pair->next;
    }

    if (jsonp_args->print_records_full)
        fprintf(stdout, "\n");
    fprintf(stdout, "}\n\n");


    /* Print keys encoding (Key : encoded key) */

    pair = record->child;
    fprintf(stdout, " Keys encoding:\n\n {");
    if (jsonp_args->print_records_full)
        fprintf(stdout, "\n");

    while (pair != NULL) {
        const unsigned char* key = pair->string;
        const long *enc_key = apr_hash_get (dict,  (const void*) key, APR_HASH_KEY_STRING);

        if (enc_key == NULL) {
            fprintf (stderr, "Error when printing dicionary\n.");
            fprintf(stderr, "The key \"%s\" does not exist.\n",  key);
            return JSONP_APR_MISSING_KEY;
        }
        if (jsonp_args->print_records_full)
            fprintf(stdout, "\t");
        fprintf(stdout, "\"%s\" : %ld", key, *enc_key);
        if (pair->next) {
            fprintf(stdout, ", ");
            if (jsonp_args->print_records_full)
                fprintf(stdout, "\n");
        }

        pair = pair->next;
    }

    if (jsonp_args->print_records_full)
        fprintf(stdout, "\n");
    fprintf(stdout, "}\n\n");

    return JSONP_SUCCESS;

}

int JSONp_PrintDict (apr_hash_t *dict) {

    apr_hash_index_t *dict_element = apr_hash_first(NULL, dict);
    while (dict_element) {
        const long *value;
        const char *key;
        apr_hash_this(dict_element, (const void**) &key, NULL, (void**) &value);
        fprintf(stdout, "\t%ld : \"%s\"", *value, key);
        if (apr_hash_next(dict_element)) fprintf(stdout, ",\n");
        else break;
    }

}

void JSONp_UpdateDict (apr_hash_t *dict, const cJSON *record,
                       JSONpArgs* jsonp_args, apr_pool_t *mp) {

    if (jsonp_args->one_dict)
        apr_hash_clear(dict);

    /* Traverse json object */
    cJSON *element = record->child;
    while (element != NULL) {
        if (apr_hash_get (dict,  (const void*) element->string,
                          APR_HASH_KEY_STRING) == NULL)
            {
                unsigned int key_number = apr_hash_count(dict) + 1;
                apr_hash_set(dict, apr_pstrdup(mp, element->string), APR_HASH_KEY_STRING, \
                             (long*) apr_pmemdup(mp, (const void *) &key_number,
                             sizeof(key_number)));
            }
        element = element->next;
    }

}


int JSONp_cJSON_print(const cJSON * record, JSONpArgs *jsonp_args) {

    char* string ;
    if (jsonp_args->print_records)
        string = cJSON_PrintUnformatted(record);
    else if (jsonp_args->print_records_full)
        string = cJSON_Print(record);
    else
        return JSONP_SUCCESS;

    if (string == NULL) {
        fprintf(stderr, "Failed to print json.\n");
        return JSONP_cJSON_PRINT_ERROR;
    }
    fprintf(stdout, "%s\n\n", string); /* Actual print goes here */

    return JSONP_SUCCESS;

}

/** @} */

