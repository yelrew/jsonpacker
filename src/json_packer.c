#include <json_packer.h>

int JSONp_Packer(const char *filepath){

    char *string = NULL;
    int status = 0;
    apr_pool_t *mp; /* apr memory pool */
    apr_hash_t *ht; /* dictionary */

    /* Open file containing JSON records */
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL) {
        fprintf(stderr, "Unable to open file \"%s\"\n", filepath);
        return 1;
    }

    /* Creating APR memory pool */
    apr_initialize();
    apr_pool_create(&mp, NULL);
    ht = apr_hash_make(mp);

    /* Parse records */
    char *record = NULL;
    size_t len = 0;
    long keys_counter = 1;
    while(getline(&record, &len, fp) != -1) {

        cJSON *json = cJSON_Parse(record);
        if (json == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) {
                fprintf(stderr, "JSON Syntax Error before: %s\n", error_ptr);
            }
            cJSON_Delete(json);
            status = 1;
            return status;
        }

        // TODO: Add current json keys to dictionary
        /* Traverse json structure */
        cJSON *element = json->child;
        while (element != NULL) {
            //printf("%s\n", (unsigned char*) element->string);
            //print_string_str((unsigned char*) element->string, stdout);
            if (apr_hash_get (ht,  (const void*) element->string, APR_HASH_KEY_STRING) == NULL) {
                apr_hash_set(ht, apr_pstrdup(mp, element->string), \
                             APR_HASH_KEY_STRING, (char*) apr_ltoa(mp, keys_counter++));
            }
            element = element->next;
        }


        // TODO: Process encoding (expect Hash table and cJSON structure)

//        apr_hash_set(ht, apr_pstrdup(mp, "key1"), APR_HASH_KEY_STRING, (int*)&val1);
//        apr_hash_set(ht, (const void*) "key3", APR_HASH_KEY_STRING, (int*)&val1);

//        /* get the value from a key */
//       {
//           const int *val = apr_hash_get(ht, "key3", APR_HASH_KEY_STRING);
//           printf("val for \"key1\" is %d\n", *val);
//       }

//        /* Print result */
//        string = cJSON_Print(json);
//        if (string == NULL) {
//            fprintf(stderr, "Failed to print json.\n");
//            status = 1;
//            return status;
//        }
//        printf("\"%s\"\n", string);
    }

    /* Checking Dictionary */
    apr_hash_index_t *hi;
    for (hi = apr_hash_first(NULL, ht); hi; hi = apr_hash_next(hi)) {
        const char *k;
        const char *v;
        apr_hash_this(hi, (const void**)&k, NULL, (void**)&v);
        printf("ht iteration: key=%s, val=%s\n", k, v);
    }

    /* The hash table is destroyed after @mp */
    apr_pool_destroy(mp);
    apr_terminate();
    fclose(fp);

    return status;
}
