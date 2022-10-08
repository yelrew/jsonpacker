#include <jsonp.h>
#include <argparser.h>

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

    char *record_str = NULL;
    size_t len = 0;
    size_t num_records = 0;

    while(getline(&record_str, &len, fp) != -1) {

        cJSON *record = cJSON_Parse(record_str);
        if (record == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) {
                fprintf(stderr, "JSON Syntax Error before: %s\n", error_ptr);
            }
            cJSON_Delete(record);
            return JSONP_cJSON_SYNTAX_ERROR;
        }

        fprintf(stdout, "/**********************************\n");
        fprintf(stdout, "Processing record %d ...\n\n", ++num_records);

        /* Print record */
        status = JSONp_cJSON_print(record);
        if (status!= JSONP_SUCCESS) break;

        /* Add current keys to dictionary */
        JSONp_UpdateDictionary(dict, record, mp);

        /* Print encoded records with keys */
        status = JSONp_PrintEncoding(dict, record);
        if (status!= JSONP_SUCCESS) break;

        /* Serializing current record */
        status = JSONp_SerializeRecord(dict, record, jsonp_args);
        if (status != JSONP_SUCCESS) break;


//        /* get the value from a key */
//       {
//           const int *val = apr_hash_get(ht, "key3", APR_HASH_KEY_STRING);
//           printf("val for \"key1\" is %d\n", *val);
//       }


    }

    /* The hash table is automatically destroyed after @mp */
    apr_pool_destroy(mp);
    apr_terminate();
    fclose(fp);

    return status;
}

int JSONp_PrintEncoding(apr_hash_t *dict, const cJSON *record) {

    cJSON *pair = record->child;

    /* Print encoded records (Encoded key : value) */

    fprintf(stdout, "Encoded records:\n{\n");

    while (pair != NULL) {
        const unsigned char* key = pair->string;
        const long *enc_key = apr_hash_get (dict,  (const void*) key, APR_HASH_KEY_STRING);

        if (enc_key == NULL) {
            fprintf (stderr, "Error when printing dicionary\n.");
            fprintf(stderr, "The key \"%s\" does not exist.\n",  key);
            return JSONP_APR_MISSING_KEY;
        }

        fprintf(stdout, "\t%ld : ", *enc_key); /* encoded key (number) */
        switch (pair->type & 0xFF) {
        case cJSON_Number:
            if (pair->valuedouble == (int) pair->valuedouble) {
                fprintf(stdout,  "%d", pair->valueint);
            } else {
                fprintf(stdout,  "%g", pair->valuedouble);
            }
            break;
        case cJSON_True:
            fprintf(stdout,  "true");
            break;
        case cJSON_False:
            fprintf(stdout,  "false");
            break;
        case cJSON_String:
            fprintf(stdout,  "\"%s\"", pair->valuestring);
            break;
        default:
            fprintf (stderr, "Error: Invalid JSON type detected!\n");
            return JSONP_cJSON_INVALID_TYPE;
        }

        if (pair->next) fprintf(stdout, ",\n");
        pair = pair->next;
    }

    fprintf(stdout, "\n}\n");

    /* Print keys encoding (Key : encoded key) */

    pair = record->child;
    fprintf(stdout, "Keys encoding:\n{\n");

    while (pair != NULL) {
        const unsigned char* key = pair->string;
        const long *enc_key = apr_hash_get (dict,  (const void*) key, APR_HASH_KEY_STRING);

        if (enc_key == NULL) {
            fprintf (stderr, "Error when printing dicionary\n.");
            fprintf(stderr, "The key \"%s\" does not exist.\n",  key);
            return JSONP_APR_MISSING_KEY;
        }
        fprintf(stdout, "\t\"%s\" : %ld", key, *enc_key);
        if (pair->next) fprintf(stdout, ",\n");

        pair = pair->next;
    }

    fprintf(stdout, "\n}\n\n");

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



void JSONp_UpdateDictionary (apr_hash_t *dict, const cJSON *record, apr_pool_t *mp) {

    static long keys_counter = 1;
    /* Traverse json structure */
    cJSON *element = record->child;
    while (element != NULL) {
        if (apr_hash_get (dict,  (const void*) element->string,
                          APR_HASH_KEY_STRING) == NULL)
            {
                apr_hash_set(dict, apr_pstrdup(mp, element->string), APR_HASH_KEY_STRING, \
                             (long*) apr_pmemdup(mp, (const void *) &keys_counter,
                             sizeof(keys_counter)));
            }
        keys_counter++;
        element = element->next;
    }

}

int JSONp_SerializeRecord(apr_hash_t *dict,
                          cJSON *record,
                          JSONpArgs* jsonp_args) {
    int status = 0;
    switch(jsonp_args->encoder) {
    case JpASN1:
        status = JSONp_ASN1Encode(record, dict);
        break;
    default:
        fprintf(stderr, "Error: unknow encoder type \"%s\"\n",jsonp_args->encoder_name);
        status = JSONP_MISSING_ENCODER;
        break;
    }
    return status;
}

int JSONp_cJSON_print(const cJSON * record) {

    char* string ;
    string = cJSON_Print(record);
    if (string == NULL) {
        fprintf(stderr, "Failed to print json.\n");
        return JSONP_cJSON_PRINT_ERROR;
    }
    fprintf(stdout, "Input record:\n\"%s\"\n\n", string);
    return JSONP_SUCCESS;

}

