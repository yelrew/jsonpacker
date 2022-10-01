#include <stdio.h>
#include <cJSON/cJSON.h>

int main() {

    const char input[] = "{ \"key1\": \"value\", \"key2\": 42, \"key3\": true}";
    int status = 0;
    char *string = NULL;
    //{“sadsf”:”dsewtew”, “dsre”:3221, “sdfds”:”dsfewew”}

    cJSON *json = cJSON_Parse(input);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "JSON Syntax Error before: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        status = 1;
        return status;
    }

    string = cJSON_Print(json);
    if (string == NULL) {
        fprintf(stderr, "Failed to print monitor.\n");
        status = 1;
        return status;
    }
    printf("\"%s\"\n", string);

    return status;

}
