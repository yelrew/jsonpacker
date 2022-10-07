#include <jsonp.h>

int JSONp_Pack(const char *filepath){

    int status = 0;
    apr_pool_t *mp; /* apr memory pool */
    apr_hash_t *dict; /* dictionary */

    /* Open file containing JSON records */
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL) {
        fprintf(stderr, "Unable to open file \"%s\"\n", filepath);
        return JSONP_FILE_OPEN_ERROR;
    }

    /* Creating APR memory pool */
    apr_initialize();
    apr_pool_create(&mp, NULL);
    dict = apr_hash_make(mp);

    /* Parse records */

    char *record_str = NULL;
    size_t len = 0;

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

        /* Update dictionary keys from current record */
        JSONp_UpdateDictionary(dict, record, mp);

        /* Serializing current record */
        JSONp_SerializeRecord(dict, record, kJsonpASN1);

        // TODO: Process encoding (expect Hash table and cJSON structure)

//        apr_hash_set(ht, apr_pstrdup(mp, "key1"), APR_HASH_KEY_STRING, (int*)&val1);
//        apr_hash_set(ht, (const void*) "key3", APR_HASH_KEY_STRING, (int*)&val1);

//        /* get the value from a key */
//       {
//           const int *val = apr_hash_get(ht, "key3", APR_HASH_KEY_STRING);
//           printf("val for \"key1\" is %d\n", *val);
//       }


    }

    /* Checking Dictionary */
//    apr_hash_index_t *hi;
//    for (hi = apr_hash_first(NULL, ht); hi; hi = apr_hash_next(hi)) {
//        const char *k;
//        const char *v;
//        apr_hash_this(hi, (const void**) &k, NULL, (void**) &v);
//        printf("ht iteration: key=%s, val=%s\n", k, v);
//    }

    /* The hash table is automatically destroyed after @mp */
    apr_pool_destroy(mp);
    apr_terminate();
    fclose(fp);

    return status;
}

void JSONp_UpdateDictionary (apr_hash_t *dict, const cJSON *record, apr_pool_t *mp) {

    static long keys_counter = 1;
    /* Traverse json structure */
    cJSON *element = record->child;
    while (element != NULL) {
        if (apr_hash_get (dict,  (const void*) element->string, APR_HASH_KEY_STRING) == NULL) {
            apr_hash_set(dict, apr_pstrdup(mp, element->string), APR_HASH_KEY_STRING, \
                           (long*) apr_pmemdup(mp, (const void *) &keys_counter, sizeof(keys_counter)));
        }
        keys_counter++;
        element = element->next;
    }

}

int JSONp_SerializeRecord(apr_hash_t *dict,
                          cJSON *record,
                          JSONpEncoder encoder_type) {
    int status =  0;
    switch(encoder_type) {

    case kJsonpASN1:
        status = JSONp_ASN1Encode(record, dict);
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
    printf("\"%s\"\n", string);
    return JSONP_SUCCESS;

}
