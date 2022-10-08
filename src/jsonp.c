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

    char *line = NULL;
    size_t len = 0;
    size_t record_num = 0;

    while(getline(&line, &len, fp) != -1) {

        cJSON *record = cJSON_Parse(line);
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

        fprintf(stdout, "/**********************************\n\n");
        fprintf(stdout, "Processing record %d ...\n\n", record_num);

        /* Print record */
        status = JSONp_cJSON_print(record, jsonp_args);
        if (status!= JSONP_SUCCESS) break;

        /* Add current keys to dictionary */
        if (jsonp_args->one_dict)
            apr_hash_clear(dict);
        JSONp_UpdateDictionary(dict, record, mp);

        /* Print encoded records with keys */
        status = JSONp_PrintEncoding(dict, record, jsonp_args);
        if (status!= JSONP_SUCCESS) break;

        /* Serializing current record */
        status = JSONp_SerializeRecord(dict, record, jsonp_args);
        if (status != JSONP_SUCCESS) break;

        cJSON_Delete(record);
    }

    /* The hash table is automatically destroyed after @mp */
    apr_pool_destroy(mp);
    apr_terminate();
    fclose(fp);

    return status;
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
        const unsigned char* key = pair->string;
        const long *enc_key = apr_hash_get (dict,  (const void*) key, APR_HASH_KEY_STRING);

        if (enc_key == NULL) {
            fprintf (stderr, "Error when printing dicionary\n.");
            fprintf(stderr, "The key \"%s\" does not exist.\n",  key);
            return JSONP_APR_MISSING_KEY;
        }

        if (jsonp_args->print_records_full)
            fprintf(stdout, "\t");
        fprintf(stdout, " %ld : ", *enc_key); /* encoded key (number) */
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


void JSONp_UpdateDictionary (apr_hash_t *dict, const cJSON *record, apr_pool_t *mp) {

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

int JSONp_SerializeRecord(apr_hash_t *dict,
                          cJSON *record,
                          JSONpArgs* jsonp_args) {
    int status = 0;
    switch(jsonp_args->encoder) {
    case JpASN1:
        status = JSONp_ASN1EncodeRecord(record, dict, jsonp_args);
        break;
    default:
        fprintf(stderr, "Error: unknow encoder type \"%s\"\n", jsonp_args->encoder_name);
        status = JSONP_MISSING_ENCODER;
        break;
    }
    return status;
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

