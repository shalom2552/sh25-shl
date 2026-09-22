#define SH25_UTEST_IMPLEMENTATION
#include "sh25_utest.h"

TEST_MAIN {
    RUN_SUITE(test_stack);
    RUN_SUITE(test_array);
}

