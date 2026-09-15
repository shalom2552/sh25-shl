/*
 * sh25_utest.h - single-header unit test library for c.
 *
 * This library provides a simple framework for writing unit tests in C/C++.
 * It provides macros to define test functions, test suites, assertions, and to run tests.
 * It also provides macros to define the main function to run all test suites.
 *
 * Define SH25_UTEST_IMPLEMENTATION in exactly ONE translation unit before including this header.
 *
 * API:
 *   TEST(name)             Define a test function. Body implement with braces.
 *   TEST_SUITE(name)       Define a test suite. Body implement with braces.
 *   ASSERT(condition)      Fail the current test if condition is false and return.
 *   ASSERT_EQ(a, b)        Fail the current test if `a != b` and report both values.
 *   RUN(name)              Run a test function, counting the number of tests passed and failed.
 *   RUN_SUITE(name)        Run a test suite, printing its results.
 *   TEST_MAIN              Generate main(). Body lists RUN_SUITE calls.
 *
 * Verbosity:
 *   (none)     full output
 *   -q (0)     suites and failures only
 *   -s (1)     silent, exit code only
 *
 * Exit Code: 0 if all tests passed, 1 otherwise.
 *
 * Usage:
 *   1. Include this header in your test file.
 *   2. Define your test functions using TEST() macro.
 *   3. Define your test suites using TEST_SUITE() macro.
 *   4. Call RUN() macro to run a test function.
 *   5. Define TEST_MAIN macro to run all test suites.
 *   6. Call RUN_SUITE() macro to run a test suite.
 *   7. Define SH25_UTEST_IMPLEMENTATION macro to implement the test functions.
 *
 * Example Usage:
 *   #define SH25_UTEST_IMPLEMENTATION
 *   #include "sh25_utest.h"
 *   #include "stack.h"
 *   TEST(test_stack_size) { ASSERT(1); }
 *   TEST_SUITE(test_stack) { RUN(test_stack_size); }
 *   TEST_MAIN { RUN_SUITE(test_stack); }
 *
 * Author: shalom2552
 * License: MIT
 */
#ifndef SH25_UTEST_H_6c5f728595d1e0c71e833e475997b1d8
#define SH25_UTEST_H_6c5f728595d1e0c71e833e475997b1d8

#include <stdio.h>

extern int sh25_utest_verbose;
extern int sh25_utest_passed;
extern int sh25_utest_failed;
extern int sh25_utest_total;

/* level; 0 = silent, 1 = suites + failures, 2 = every test (default) */
#define SH25_LOG(level, ...) if (sh25_utest_verbose >= level) { printf(__VA_ARGS__); fflush(stdout); }

#define TEST(name) static void name(void)
#define TEST_SUITE(name) void name(void)

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            SH25_LOG(1, " [FAIL]\n%s:%d: %s\n", __FILE__, __LINE__, #condition); \
            sh25_utest_failed = 1; \
            return; \
        } \
    } while (0)

#define ASSERT_EQ(actual, expected) \
    do { \
        long sh25_a = (long)actual; \
        long sh25_e = (long)expected; \
        if (sh25_a != sh25_e) { \
            SH25_LOG(1, " \t[FAIL]\n%s:%d: %s == %s: got: %ld, expected: %ld\n", \
                     __FILE__, __LINE__, #actual, #expected, sh25_a, sh25_e); \
            sh25_utest_failed = 1; \
            return; \
        } \
    } while (0)

#define RUN_TEST(name) \
    do { \
        sh25_utest_failed = 0; \
        sh25_utest_total++; \
        SH25_LOG(2, "  Running test: %s...", #name); \
        name(); \
        if (!sh25_utest_failed) { \
            sh25_utest_passed++; \
            SH25_LOG(2, " \t[OK]\n"); \
        } \
    } while (0)

#define RUN_SUITE(name) \
    do { \
        void name(void); \
        int passed = sh25_utest_passed; \
        int total = sh25_utest_total; \
        SH25_LOG(1, "Running suite: %s...\n", #name); \
        name(); \
        SH25_LOG(1, "%s: %d/%d tests passed!\n\n", #name, sh25_utest_passed - passed, sh25_utest_total - total); \
    } while (0) \

#define TEST_MAIN \
    static void sh25_utest_run(void); \
    int main (int argc, char *argv[]) { \
        for (int i = 1; i < argc; i++) { \
             if (argv[i][0] == '-' && argv[i][1] == 'q') sh25_utest_verbose = 1; \
             if (argv[i][0] == '-' && argv[i][1] == 's') sh25_utest_verbose = 0; \
        } \
        SH25_LOG(1, "SH25 Unit Tests\n"); \
        SH25_LOG(1, "=================================================\n"); \
        sh25_utest_run(); \
        SH25_LOG(2, "=================================================\n"); \
        SH25_LOG(1, "Total %d/%d tests passed!\n", sh25_utest_passed, sh25_utest_total); \
        return sh25_utest_passed == sh25_utest_total ? 0 : 1; \
    } \
    static void sh25_utest_run(void)

#ifdef SH25_UTEST_IMPLEMENTATION
int sh25_utest_verbose = 2;
int sh25_utest_failed = 0;
int sh25_utest_passed = 0;
int sh25_utest_total = 0;
#endif // SH25_UTEST_IMPLEMENTATION

#endif // !SH25_UTEST_H_6c5f728595d1e0c71e833e475997b1d8

