#define SH25_UTEST_IMPLEMENTATION
#include "../include/sh25_utest.h"

#define NO_MAIN
#include "stack_example.c"

TEST(test_valid_parentheses_valid_empty) {
    ASSERT(valid_parentheses("") == 1);
}

TEST(test_valid_parentheses_valid_single) {
    ASSERT(valid_parentheses("()") == 1);
}

TEST(test_valid_parentheses_valid_multiple) {
    ASSERT(valid_parentheses("([])") == 1);
}

TEST(test_valid_parentheses_valid_long) {
    ASSERT(valid_parentheses("{([])}") == 1);
}

TEST(test_valid_parentheses_valid_longest) {
    ASSERT(valid_parentheses("()()[()]{{()[()]}}") == 1);
}

TEST_SUITE(test_valid_parentheses_valid) {
    RUN_TEST(test_valid_parentheses_valid_empty);
    RUN_TEST(test_valid_parentheses_valid_single);
    RUN_TEST(test_valid_parentheses_valid_multiple);
    RUN_TEST(test_valid_parentheses_valid_long);
    RUN_TEST(test_valid_parentheses_valid_longest);
}

TEST(test_valid_parentheses_invalid_single) {
    ASSERT(valid_parentheses("(") == 0);
}

TEST(test_valid_parentheses_invalid_multiple) {
    ASSERT(valid_parentheses("((") == 0);
}

TEST(test_valid_parentheses_invalid_long) {
    ASSERT(valid_parentheses("([)]") == 0);
}

TEST(test_valid_parentheses_invalid_longest) {
    ASSERT(valid_parentheses("{([[])}]") == 0);
}

TEST_SUITE(test_valid_parentheses_invalid) {
    RUN_TEST(test_valid_parentheses_invalid_single);
    RUN_TEST(test_valid_parentheses_invalid_multiple);
    RUN_TEST(test_valid_parentheses_invalid_long);
    RUN_TEST(test_valid_parentheses_invalid_longest);
}

TEST_MAIN {
    RUN_SUITE(test_valid_parentheses_valid);
    RUN_SUITE(test_valid_parentheses_invalid);
}

