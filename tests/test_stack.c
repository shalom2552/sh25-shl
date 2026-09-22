#define SH25_STACK_IMPLEMENTATION
#include "sh25_stack.h"

#include "sh25_utest.h"

TEST(test_stack_init)
{
    Stack stack;
    stack_init(stack, sizeof(int));
    ASSERT_EQ(stack.size, 0);

    stack_destroy(stack);
}

TEST(test_stack_push)
{
    Stack stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    stack_push(stack, a);
    ASSERT_EQ(stack.size, 1);

    int pop;
    stack_pop(stack, pop);
    ASSERT_EQ(pop, a);
    ASSERT_EQ(stack.size, 0);

    stack_destroy(stack);
}

TEST(test_stack_push_same_element)
{
    Stack stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    stack_push(stack, a);
    ASSERT_EQ(stack.size, 1);

    int pop;
    stack_pop(stack, pop);
    ASSERT_EQ(pop, a);
    ASSERT_EQ(stack.size, 0);

    stack_destroy(stack);
}

TEST(test_stack_push_twice)
{
    Stack stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    int b = 2;
    stack_push(stack, a);
    ASSERT_EQ(stack.size, 1);
    stack_push(stack, b);
    ASSERT_EQ(stack.size, 2);

    int pop;
    stack_pop(stack, pop);
    ASSERT_EQ(pop, b);
    ASSERT_EQ(stack.size, 1);

    stack_pop(stack, pop);
    ASSERT_EQ(pop, a);
    ASSERT_EQ(stack.size, 0);

    stack_destroy(stack);
}

TEST(test_stack_peek)
{
    Stack stack;
    stack_init(stack, sizeof(int));

    int peek;
    int a = 1;
    int b = 2;
    stack_push(stack, a);

    stack_peek(stack, peek);
    ASSERT_EQ(peek, a);
    ASSERT_EQ(stack.size, 1);

    stack_push(stack, b);
    ASSERT_EQ(stack.size, 2);

    stack_peek(stack, peek);
    ASSERT_EQ(peek, b);
    ASSERT_EQ(stack.size, 2);

    stack_destroy(stack);
}

TEST(test_stack_clear)
{
    Stack stack;
    stack_init(stack, sizeof(int));

    int value = 5;
    int a = 1;
    stack_push(stack, a);

    stack_clear(stack);
    ASSERT_EQ(stack_size(stack), 0);
    ASSERT(stack_empty(stack));
    ASSERT_EQ(stack.size, 0);

    stack_peek(stack, value);
    ASSERT_EQ(value, 5);
    ASSERT_EQ(stack.size, 0);

    stack_destroy(stack);
}

TEST(test_stack_drop)
{
    Stack stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    stack_push(stack, a);
    ASSERT_EQ(stack.size, 1);
    stack_drop(stack);
    ASSERT_EQ(stack.size, 0);
    ASSERT(stack_empty(stack));
    stack_destroy(stack);
}

TEST(test_stack_size)
{
    Stack stack;
    stack_init(stack, sizeof(int));

    int a = 1;
    for (int i = 0; i < STACK_INITIAL_CAPACITY * 2 + 2; ++i) {
        ASSERT_EQ(stack.size, (size_t)i);
        ASSERT_EQ(stack_size(stack), i);
        stack_push(stack, a);
    }
    stack_destroy(stack);
}

// === MAIN ===================================================================
TEST_SUITE(test_stack)
{
    RUN_TEST(test_stack_init);
    RUN_TEST(test_stack_push);
    RUN_TEST(test_stack_push_same_element);
    RUN_TEST(test_stack_push_twice);
    RUN_TEST(test_stack_peek);
    RUN_TEST(test_stack_clear);
    RUN_TEST(test_stack_drop);
    RUN_TEST(test_stack_size);
}

