#include <assert.h>
#include <stdio.h>
#include <string.h>
#define SH25_ARRAY_IMPL
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
    ASSERT_EQ(array_empty(array), 1);
    ASSERT_EQ(array_size(array), 0);

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
    size_t max_size = ARRAY_INITIAL_CAPACITY * 2 + 2;

    int value;
    for (int i = 0; (size_t)i < max_size; ++i) {
        ASSERT_EQ(array.size, (size_t)i);
        ASSERT_EQ(array_size(array), i);
        array_append(array, i);
    }
    for (size_t i = max_size; i > 0; --i) {
        size_t idx = i - 1;
        array_get(array, idx, value);
        ASSERT_EQ(value, idx);
        ASSERT_EQ(array.size, max_size);
        ASSERT_EQ(array_size(array), max_size);
    }

    array_clear(array);
    for (int i = 0; (size_t)i < max_size; ++i) {
        ASSERT_EQ(array.size, (size_t)i);
        ASSERT_EQ(array_size(array), i);
        array_append(array, (int)i);
    }
    for (size_t i = max_size; i > 0; --i) {
        size_t idx = i - 1;
        array_get(array, idx, value);
        ASSERT_EQ(value, idx);
        ASSERT_EQ(array.size, max_size);
        ASSERT_EQ(array_size(array), max_size);
    }
    array_destroy(array);
}

TEST(test_array_special_type)
{
    struct Person {
        char* name;
        int age;
    } p = {
        .name = "Name",
        .age = 25
    };

    Array array;
    array_init(array, sizeof(struct Person));

    struct Person value;
    size_t max_size = ARRAY_INITIAL_CAPACITY * 2 + 2;

    for (size_t i = 0; i < max_size; ++i) {
        ASSERT_EQ(array.size, (size_t)i);
        ASSERT_EQ(array_size(array), i);
        array_append(array, p);
    }
    for (size_t i = max_size; i > 0; --i) {
        size_t idx = i - 1;
        array_get(array, idx, value);
        ASSERT_EQ(value.name, p.name);
        ASSERT_EQ(value.age, p.age);
        ASSERT_EQ(array.size, max_size);
        ASSERT_EQ(array_size(array), max_size);
    }

    array_destroy(array);
}

TEST(test_array_swap_remove)
{
    Array array;
    array_init(array, sizeof(char*));
    char* val_1 = "first element";
    char* val_2 = "second element";
    char* val_3 = "third element";
    ASSERT_EQ(array_append(array, val_1), ARRAY_OK);
    ASSERT_EQ(array_append(array, val_2), ARRAY_OK);
    ASSERT_EQ(array_append(array, val_3), ARRAY_OK);
    ASSERT_EQ(array_size(array), 3);

    char* element;
    array_get(array, 1, element);
    ASSERT_EQ(strcmp(val_2, element), 0);

    ASSERT_EQ(array_swap_remove(array, 0), ARRAY_OK);
    ASSERT_EQ(array_size(array), 2);
    ASSERT_EQ(array_get(array, 0, element), ARRAY_OK);
    ASSERT_EQ(strcmp(element, val_3), 0);

    array_destroy(array);
}

TEST(test_array_shift_remove)
{
    Array array;
    array_init(array, sizeof(int));

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(array_size(array), i);
        array_append(array, i);
    }

    ASSERT_EQ(array_shift_remove(array, 0), ARRAY_OK);
    int val;
    for (int i = 0; i < 2; ++i) {
        array_get(array, i, val);
        ASSERT_EQ(val, i + 1);
    }

    array_destroy(array);
}

TEST(test_array_remove_last_shift_and_swap)
{
    Array array;
    array_init(array, sizeof(int));
    for (int i = 1; i <= 4; ++i) {
        array_append(array, i);
    }
    ASSERT_EQ(array_swap_remove(array,  2), ARRAY_OK);
    ASSERT_EQ(array_shift_remove(array, 1), ARRAY_OK);
    ASSERT_EQ(array_swap_remove(array,  0), ARRAY_OK);
    ASSERT_EQ(array_append(array,       0), ARRAY_OK);
    ASSERT_EQ(array_shift_remove(array, 1), ARRAY_OK);
    array_destroy(array);
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
    RUN_TEST(test_array_swap_remove);
    RUN_TEST(test_array_shift_remove);
    RUN_TEST(test_array_remove_last_shift_and_swap);
}

