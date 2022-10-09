#include "unity.h"
#include "jsonp.h"

static void assert_length_equal_min_signed_int_length(void) {

    /*
        39: 00000000 00000000 00000000 00100111 (0x00000027) 1
       -39: 11111111 11111111 11111111 11011001 (0xffffffd9) 1
      -128: 11111111 11111111 11111111 10000000 (0xffffff80) 1
       255: 00000000 00000000 00000000 11111111 (0x000000ff) 2
    223498: 00000000 00000011 01101001 00001010 (0x0003690a) 3
     -1098: 11111111 11111111 11111011 10110110 (0xfffffbb6) 2
         1: 00000000 00000000 00000000 00000001 (0x00000001) 1
         0: 10000000 00000000 00000000 00000000 0 (0x100000000) 1
        -1: 11111111 11111111 11111111 11111111 (0xffffffff) 1
     */

    TEST_ASSERT_EQUAL(1, MinSignedIntLength(39));
    TEST_ASSERT_EQUAL(1, MinSignedIntLength(-39));
    TEST_ASSERT_EQUAL(1, MinSignedIntLength(-128));
    TEST_ASSERT_EQUAL(2, MinSignedIntLength(255));
    TEST_ASSERT_EQUAL(3, MinSignedIntLength(223498));
    TEST_ASSERT_EQUAL(2, MinSignedIntLength(-1098));
    TEST_ASSERT_EQUAL(1, MinSignedIntLength(1));
    TEST_ASSERT_EQUAL(1, MinSignedIntLength(0));
    TEST_ASSERT_EQUAL(1, MinSignedIntLength(1));
}

int test_jsonp_asn1() {

    UNITY_BEGIN();
    RUN_TEST(assert_length_equal_min_signed_int_length);
    return UNITY_END();

}
