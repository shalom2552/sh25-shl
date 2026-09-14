#define SH25_STACK_IMPLEMENTATION
#include "../sh25_stack.h"

#define RUN(fn) (fn(), ++count)
#include <assert.h>

static void test_stack_init(void)
{
    stack_t stack;
    stack_init(stack, sizeof(int));
    assert(stack.capacity == STACK_INITIAL_CAPACITY);
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

// === MAIN ===================================================================
int test_stack(void)
{
    int count = 0;
    RUN(test_stack_init);
    RUN(test_stack_push);
    RUN(test_stack_push_same_element);
    RUN(test_stack_push_twice);
    return count;
}

