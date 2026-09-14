#include <stdio.h>
#include <unistd.h>

int test_stack(void);

static int total = 0;
#define TEST(fn) test(fn, #fn)

static void test(int (*test_suite)(void), const char* name)
{
    printf("Running %s...", name);
    fflush(stdout);

    int count = test_suite();
    total += count;

    printf("\r\033[2K %s: \t%d tests [OK]\n", name, count);
}

int main(void)
{
    printf("sh25 unit tests\n");
    printf("=================================================\n");

    TEST(test_stack);

    printf("=================================================\n");
    printf("\nTotal %d tests passed!\n", total);
    return 0;
}

