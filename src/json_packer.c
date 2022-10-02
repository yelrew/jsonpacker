#include <json_packer.h>

int JSONp_Packer(const char *filepath){

    char *string = NULL;
    int status = 0;

    /* Open file containing JSON records */
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL) {
        fprintf(stderr, "Unable to open file \"%s\"\n", filepath);
        return 1;
    }

    /* Parse records */
    char *record = NULL;
    size_t len = 0;
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

        // TODO: Update HashTable with record keys
        // TODO: Process encoding (expect Hash table and cJSON structure)

//        /* Print result */
//        string = cJSON_Print(json);
//        if (string == NULL) {
//            fprintf(stderr, "Failed to print json.\n");
//            status = 1;
//            return status;
//        }
//        printf("\"%s\"\n", string);
    }

    fclose(fp);

    return status;
}
