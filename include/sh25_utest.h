/*
 * sh25_utest.h - single-header unit test library for c.
 *
 * This library provides a simple framework for writing unit tests in C/C++.
 * It provides macros to define test functions, test suites, assertions, and to run tests.
 * It also provides macros to define the main function to run all test suites.
 *
 * Define SH25_UTEST_IMPL in exactly ONE translation unit before including this header.
 *
 * API:
 *   TEST(name)             Define a test function. Body implement with braces.
 *   TEST_SUITE(name)       Define a test suite. Body implement with braces.
 *   ASSERT(condition)      Fail the current test if condition is false and return.
 *   ASSERT_EQ(a, b)        Fail the current test if `a != b` and report both values.
 *   RUN_TEST(name)         Run a test function, counting the number of tests passed and failed.
 *   RUN_SUITE(name)        Run a test suite, printing its results.
 *   TEST_MAIN              Generate main(). Body lists RUN_SUITE calls.
 *
 * Verbosity:
 *   (none)     full output
 *   -q (1)     failures and summary only
 *   -s (0)     silent, exit code only
 *
 * Exit Code: 0 if all tests passed, 1 otherwise.
 *
 * Usage:
 *   1. Define SH25_UTEST_IMPL once, before including this header.
 *   2. Define tests with TEST().
 *   3. Define suites with TEST_SUITE(), call RUN_TEST() for each test.
 *   4. Use TEST_MAIN once, call RUN_SUITE() for each suite.
 *
 * run the tests:
 *   $ gcc -o test test_*.c
 *   $ ./test
 *
 * Example Usage:
 *   #define SH25_UTEST_IMPL
 *   #include "sh25_utest.h"
 *   #include "stack.h"
 *   TEST(test_stack_size) { ASSERT(1); }
 *   TEST_SUITE(test_stack) { RUN_TEST(test_stack_size); }
 *   TEST_MAIN { RUN_SUITE(test_stack); }
 *
 * Author: shalom2552
 * License: MIT
 */
#ifndef SH25_UTEST_H_6c5f728595d1e0c71e833e475997b1d8
#define SH25_UTEST_H_6c5f728595d1e0c71e833e475997b1d8

#include <stdarg.h>
#include <stdio.h>

#ifndef SH25_UTEST_MAX_FAILURES
#define SH25_UTEST_MAX_FAILURES 64
#endif // !SH25_UTEST_MAX_FAILURES

#ifndef SH25_UTEST_FAILURE_MSG_LEN
#define SH25_UTEST_FAILURE_MSG_LEN 512
#endif // !SH25_UTEST_FAILURE_MSG_LEN

#if defined(__FILE_NAME__)
#define SH25__FILE __FILE_NAME__
#else
#define SH25__FILE __FILE__
#endif

extern int sh25_utest_verbose;
extern int sh25_utest_suites;
extern int sh25_utest_passed;
extern int sh25_utest_failed;
extern int sh25_utest_total;

extern const char* sh25_utest_test_name;
void sh25_utest__record(const char* fmt, ...);
void sh25_utest__report(void);

#define SH25__DOTS "..................................................."
#define SH25__LINE "============================================================"

/* level; 0 = silent, 1 = failures + summary, 2 = every test (default) */
#define SH25__LOG(level, ...) do { if (sh25_utest_verbose >= level) { printf(__VA_ARGS__); fflush(stdout); } } while (0)

#define TEST(name) static void name(void)
#define TEST_SUITE(name) void name(void)

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            SH25__LOG(2, " [FAIL]\n"); \
            sh25_utest__record("%s:%d: %s", SH25__FILE, __LINE__, #condition); \
            sh25_utest_failed = 1; \
            return; \
        } \
    } while (0)

#define ASSERT_EQ(actual, expected) \
    do { \
        long sh25_a = (long)(actual); \
        long sh25_e = (long)(expected); \
        if (sh25_a != sh25_e) { \
            SH25__LOG(2, " [FAIL]\n"); \
            sh25_utest__record("%s:%d: %s == %s\n    got:  %ld\n    want: %ld", \
                     SH25__FILE, __LINE__, #actual, #expected, sh25_a, sh25_e); \
            sh25_utest_failed = 1; \
            return; \
        } \
    } while (0)

#define RUN_TEST(name) \
    do { \
        sh25_utest_test_name = #name; \
        sh25_utest_failed = 0; \
        sh25_utest_total++; \
        SH25__LOG(2, "  %s %.*s", #name, \
                    sizeof(#name) >= sizeof(SH25__DOTS) ? 1 : \
                    (int)(sizeof(SH25__DOTS) - sizeof(#name)), SH25__DOTS); \
        name(); \
        if (!sh25_utest_failed) { \
            sh25_utest_passed++; \
            SH25__LOG(2, " [OK]\n"); \
        } \
    } while (0)

#define RUN_SUITE(name) \
    do { \
        void name(void); \
        int sh25_old_p = sh25_utest_passed; \
        int sh25_old_t = sh25_utest_total; \
        sh25_utest_suites++; \
        SH25__LOG(2, "\n%s\n", #name); \
        name(); \
        SH25__LOG(2, "  %d/%d passed\n", \
                  sh25_utest_passed - sh25_old_p, \
                  sh25_utest_total - sh25_old_t); \
    } while (0)

#define TEST_MAIN \
    static void sh25_utest_run(void); \
    int main (int argc, char *argv[]) { \
        for (int i = 1; i < argc; i++) { \
             if (argv[i][0] == '-' && argv[i][1] == 'q') sh25_utest_verbose = 1; \
             if (argv[i][0] == '-' && argv[i][1] == 's') sh25_utest_verbose = 0; \
        } \
        SH25__LOG(1, "SH25 Unit Tests\n"); \
        SH25__LOG(1, SH25__LINE"\n"); \
        sh25_utest_run(); \
        SH25__LOG(2, "\n"SH25__LINE"\n"); \
        sh25_utest__report(); \
        SH25__LOG(1, "%d %s, %d passed, %d failed\n", \
                  sh25_utest_suites, sh25_utest_suites != 1 ? "suites" : "suite", \
                  sh25_utest_passed, sh25_utest_total - sh25_utest_passed); \
        return sh25_utest_passed == sh25_utest_total ? 0 : 1; \
    } \
    static void sh25_utest_run(void)

#ifdef SH25_UTEST_IMPL
int sh25_utest_verbose = 2;
int sh25_utest_nfailures = 0;
int sh25_utest_suites = 0;
int sh25_utest_failed = 0;
int sh25_utest_passed = 0;
int sh25_utest_total = 0;
const char* sh25_utest_test_name = "";

static char sh25_utest_failures[SH25_UTEST_MAX_FAILURES][SH25_UTEST_FAILURE_MSG_LEN];

void sh25_utest__record(const char* fmt, ...)
{
    va_list ap;
    if (sh25_utest_nfailures >= SH25_UTEST_MAX_FAILURES) return;
    int n = snprintf(sh25_utest_failures[sh25_utest_nfailures],
                    SH25_UTEST_FAILURE_MSG_LEN, "%s\n    ", sh25_utest_test_name);
    va_start(ap, fmt);
    vsnprintf(sh25_utest_failures[sh25_utest_nfailures] + n, SH25_UTEST_FAILURE_MSG_LEN - n, fmt, ap);
    va_end(ap);
    sh25_utest_nfailures++;
}

void sh25_utest__report(void)
{
    if (sh25_utest_verbose < 1 || sh25_utest_nfailures == 0) return;
    fprintf(stderr, "FAILURES\n");
    for (int i = 0; i < sh25_utest_nfailures; i++) {
        fprintf(stderr, "\n  %s\n", sh25_utest_failures[i]);
    }
    fprintf(stderr, "\n"SH25__LINE"\n");
}

#endif // SH25_UTEST_IMPL

#endif // !SH25_UTEST_H_6c5f728595d1e0c71e833e475997b1d8

