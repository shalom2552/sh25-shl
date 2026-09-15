#include <stdio.h>
#include <unistd.h>

int test_stack(void);

static int total = 0;
#define TEST(fn) test(fn, #fn)

static void test(int (*test_suite)(void), const char* name)
{
    printf("Running %s...\n", name);

    int count = test_suite();
    total += count;

    printf("%s: \t%d tests passed\n\n", name, count);
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

