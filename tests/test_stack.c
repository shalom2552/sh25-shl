#define SH25_STACK_IMPLEMENTATION
#include "sh25_stack.h"

#include <stddef.h>
#include <assert.h>
#include <stdio.h>

#define RUN(fn) (test(fn, #fn), ++count)

static void test(void (*unit_test)(void), const char* name)
{
    printf("  Running %s...", name);
    fflush(stdout);
    unit_test();
    printf(" [OK]\n");
}

static void test_stack_init(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));
    assert(stack.size == 0);

    stack_destroy(stack);
}

static void test_stack_push(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    stack_push(stack, a);
    assert(stack.size == 1);

    int pop;
    stack_pop(stack, pop);
    assert(pop == a);
    assert(stack.size == 0);

    stack_destroy(stack);
}

static void test_stack_push_same_element(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    stack_push(stack, a);
    assert(stack.size == 1);

    int pop;
    stack_pop(stack, pop);
    assert(pop == a);
    assert(stack.size == 0);

    stack_destroy(stack);
}

static void test_stack_push_twice(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    int b = 2;
    stack_push(stack, a);
    assert(stack.size == 1);
    stack_push(stack, b);
    assert(stack.size == 2);

    int pop;
    stack_pop(stack, pop);
    assert(pop == b);
    assert(stack.size == 1);

    stack_pop(stack, pop);
    assert(pop == a);
    assert(stack.size == 0);

    stack_destroy(stack);
}

static void test_stack_peek(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));

    int peek;
    int a = 1;
    int b = 2;
    stack_push(stack, a);

    stack_peek(stack, peek);
    assert(peek == a);
    assert(stack.size == 1);

    stack_push(stack, b);
    assert(stack.size == 2);

    stack_peek(stack, peek);
    assert(peek == b);
    assert(stack.size == 2);

    stack_destroy(stack);
}

static void test_stack_clear(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));

    int value = 5;
    int a = 1;
    stack_push(stack, a);

    stack_clear(stack);
    assert(stack_size(stack) == 0);
    assert(stack_empty(stack));
    assert(stack.size == 0);

    stack_peek(stack, value);
    assert(value == 5);
    assert(stack.size == 0);

    stack_destroy(stack);
}

static void test_stack_drop(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    stack_push(stack, a);
    assert(stack.size == 1);
    stack_drop(stack);
    assert(stack.size == 0);
    assert(stack_empty(stack));
    stack_destroy(stack);
}

static void test_stack_size(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    for (int i = 0; i < STACK_INITIAL_CAPACITY * 2 + 2; ++i) {
        assert(stack.size == (size_t)i);
        assert(stack_size(stack) == i);
        stack_push(stack, a);
    }
    stack_destroy(stack);
}

// === MAIN ===================================================================
int test_stack(void)
{
    int count = 0;
    RUN(test_stack_init);
    RUN(test_stack_push);
    RUN(test_stack_push_same_element);
    RUN(test_stack_push_twice);
    RUN(test_stack_peek);
    RUN(test_stack_clear);
    RUN(test_stack_drop);
    RUN(test_stack_size);
    return count;
}

