#include <assert.h>
#define SH25_ARRAY_IMPLEMENTATION
#include "sh25_array.h"

#include "sh25_utest.h"

#include <stddef.h>

TEST(test_array_init)
{
    Array array;
    array_init(array, sizeof(int));
    ASSERT_EQ(array.size, 0);

    int value = 0;
    ASSERT_EQ(array_get(array, 0, value), ARRAY_OUT_OF_BOUND);
    ASSERT_EQ(array_set(array, 0, value), ARRAY_OUT_OF_BOUND);

    array_destroy(array);
}

TEST(test_array_append)
{
    Array array;
    array_init(array, sizeof(int));

    int a = 1;
    array_append(array, a);
    ASSERT_EQ(array.size, 1);

    int value;
    array_get(array, 0, value);
    ASSERT_EQ(value, a);
    ASSERT_EQ(array.size, 1);

    array_destroy(array);
}

TEST(test_array_append_same_element)
{
    Array array;
    array_init(array, sizeof(int));

    int a = 1;
    int value = 0;
    array_append(array, a);
    ASSERT_EQ(array.size, 1);
    array_get(array, 0, value);
    ASSERT_EQ(value, a);

    value = 0;
    array_append(array, a);
    ASSERT_EQ(array.size, 2);
    array_get(array, 1, value);
    ASSERT_EQ(value, a);

    array_destroy(array);
}

TEST(test_array_append_twice)
{
    Array array;
    array_init(array, sizeof(int));

    int a = 1;
    int b = 2;
    int value = 0;
    array_append(array, a);
    ASSERT_EQ(array.size, 1);
    array_get(array, 0, value);
    ASSERT_EQ(value, a);

    value = 0;
    array_append(array, b);
    ASSERT_EQ(array.size, 2);
    array_get(array, 1, value);
    ASSERT_EQ(value, b);

    array_destroy(array);
}

TEST(test_array_clear)
{
    Array array;
    array_init(array, sizeof(int));

    int a = 1;

    array_append(array, a);
    array_clear(array);

    ASSERT_EQ(array.size, 0);
    ASSERT_EQ(array_size(array), 0);
    ASSERT(array_empty(array));

    int value = 5;
    ArrayResult res = array_get(array, 0, value);
    ASSERT_EQ(res, ARRAY_OUT_OF_BOUND);
    ASSERT_EQ(value, 5);
    ASSERT_EQ(array.size, 0);

    array_destroy(array);
}

TEST(test_array_size)
{
    Array array;
    array_init(array, sizeof(int));
    int max_size = ARRAY_INITIAL_CAPACITY * 2 + 2;

    int value;
    for (int i = 0; i < max_size; ++i) {
        ASSERT_EQ(array.size, (size_t)i);
        ASSERT_EQ(array_size(array), i);
        array_append(array, i);
    }
    for (int i = max_size - 1; i >= 0; --i) {
        array_get(array, i, value);
        ASSERT_EQ(value, i);
        ASSERT_EQ(array.size, (size_t)i);
        ASSERT_EQ(array_size(array), i);
    }

    array_clear(array);
    for (int i = 0; i < max_size; ++i) {
        ASSERT_EQ(array.size, (size_t)i);
        ASSERT_EQ(array_size(array), i);
        array_append(array, i);
    }
    for (int i = max_size - 1; i >= 0; --i) {
        array_get(array, 0, value);
        ASSERT_EQ(value, i);
        ASSERT_EQ(array.size, (size_t)i);
        ASSERT_EQ(array_size(array), i);
    }
    array_destroy(array);
}

TEST(test_array_special_type)
{
    ASSERT(0 && "TODO: test special");
}

// === MAIN ===================================================================
TEST_SUITE(test_array)
{
    RUN_TEST(test_array_init);
    RUN_TEST(test_array_append);
    RUN_TEST(test_array_append_same_element);
    RUN_TEST(test_array_append_twice);
    RUN_TEST(test_array_clear);
    RUN_TEST(test_array_size);
    RUN_TEST(test_array_special_type);
}

