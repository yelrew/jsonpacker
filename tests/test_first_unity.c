#include "unity.h"
#include <stdio.h>


int8_t AverageThreeBytes(int8_t a, int8_t b, int8_t c)
{
    return (int8_t)(((int16_t)a + (int16_t)b + (int16_t)c) / 3);
}

void test_AverageThreeBytes_should_AverageHighValues()
{
    TEST_ASSERT_EQUAL_HEX8(80, AverageThreeBytes(50, 80, 90));
    TEST_ASSERT_EQUAL_HEX8(127, AverageThreeBytes(127, 127, 127));
    TEST_ASSERT_EQUAL_HEX8(84, AverageThreeBytes(0, 126, 126));
}

void setUp(void) {}
void tearDown(void) {}

int test_first_unity() {

    UNITY_BEGIN();
    RUN_TEST(test_AverageThreeBytes_should_AverageHighValues);
    return UNITY_END();

}
