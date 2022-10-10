#include "unity.h"
#include "jsonp.h"

static void asn1_encoder_matches_expected_bytearray(void) {

    char *string;
    cJSON *record;
    apr_hash_t *dict;
    apr_pool_t *mp;
    JSONpArgs jsonp_args;
    Asn1Array *encValue = NULL, *keyEnc = NULL;

    test_string = "{\"key1\":\"value\", \"key2\": 42, \"key3\": true}";
    record = cJSON_Parse(test_string);
    jsonp_args.encoder = JpASN1;
    jsonp_args.print_encodings = false;
    jsonp_args.print_records = false;
    jsonp_args.print_records_full = false;
    jsonp_args.write_binary_files = false;

    apr_initialize();
    apr_pool_create(&mp, NULL);
    dict = apr_hash_make(mp);
    JSONp_UpdateDict(dict, record, &jsonp_args, mp);

    encValue = malloc (sizeof(Asn1Array));
    Asn1Array_Init(encValue, "values", record->string);
    keyEnc = malloc (sizeof(Asn1Array));
    Asn1Array_Init(keyEnc, "keys", record->string);

    char *encValue1_expected = "\x30\x1c\x30\x0a\x02\x01\x01\x0c\x05\x76\x61\x6c"
                               "\x75\x65\x30\x06\x02\x01\x02\x02\x01\x2a\x30\x06"
                               "\x02\x01\x03\x01\x01\xff";

    char *keyEnc1_expected = "\x30\x21\x30\x09\x0c\x04\x6b\x65\x79\x31\x02\x01\x01"
                             "\x30\x09\x0c\x04\x6b\x65\x79\x32\x02\x01\x02\x30\x09"
                             "\x0c\x04\x6b\x65\x79\x33\x02\x01\x03";


    TEST_ASSERT_EQUAL_INT(JSONp_EncodeRecord(dict, record, &jsonp_args, encValue, keyEnc), 0);
    TEST_ASSERT_EQUAL_STRING(encValue1_expected, encValue->data);
    TEST_ASSERT_EQUAL_STRING(keyEnc1_expected, keyEnc->data);

    cJSON_Delete(record);
    apr_pool_destroy(mp);
    apr_terminate();
    Asn1Array_Clear(keyEnc);
    Asn1Array_Clear(encValue);
    free(keyEnc);
    free(encValue);
}

static void invalid_encoder_choice(void) {

    char *string = "{\"key1\":\"value\", \"key2\": 42, \"key3\": true}";
    cJSON *record = cJSON_Parse(string);
    JSONpArgs jsonp_args;
    jsonp_args.encoder = GnuASN1; /* Invalid choice */
    jsonp_args.print_encodings = false;
    jsonp_args.print_records = false;
    jsonp_args.print_records_full = false;
    jsonp_args.write_binary_files = false;

    TEST_ASSERT(JSONp_EncodeRecord(NULL, record, &jsonp_args, NULL, NULL));

    cJSON_Delete(record);

}

int test_jsonp() {

    UNITY_BEGIN();
    // RUN_TEST(invalid_encoder_choice);
    RUN_TEST(asn1_encoder_matches_expected_bytearray);
    return UNITY_END();

}
