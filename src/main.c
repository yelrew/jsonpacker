#include <stdio.h>
#include <cJSON/cJSON.h>

int main() {

    const char record1[] = "{ \"key1\": \"value\", \"key2\": 42, \"key3\": true}";
    const char record2[] = "{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}";
    int status = 0;
    char *string = NULL;

    /* Parse JSON from record1 */
    cJSON *json = cJSON_Parse(record1);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON Syntax Error before: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        status = 1;
        return status;
    }

    /* Print result */
    string = cJSON_Print(json);
    if (string == NULL) {
        fprintf(stderr, "Failed to print json.\n");
        status = 1;
        return status;
    }
    printf("\"%s\"\n", string);

    return status;

}
