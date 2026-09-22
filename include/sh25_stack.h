/*
 * sh25_stack.h - single-header stack library for c.
 *
 * This library provides a simple stack implementation with dynamic memory allocation.
 * The stack is implemented using a dynamic array and supports dynamic resizing.
 *
 * Define SH25_STACK_IMPLEMENTATION in exactly ONE translation unit before including this header.
 *
 * API:
 *   stack_init(stack, item_size)   initialize the stack with a given item size, must call before use.
 *   stack_push(stack, item)        Pushes an item to the stack, returns STACK_MEMORY_ERROR on failed allocation.
 *   stack_pop(stack, pop)          Pops into pop, returns STACK_EMPTY if empty.
 *   stack_drop(stack)              Pops without storing the value.
 *   stack_peek(stack, peek)        Copies top into peek, returns STACK_EMPTY if empty.
 *   stack_size(stack)              Returns the number of items in the stack.
 *   stack_empty(stack)             Returns 1 if the stack is empty, 0 otherwise.
 *   stack_clear(stack)             Clears the stack and keeps the allocated capacity.
 *   stack_destroy(stack)           Destroys the stack and frees all allocated memory.
 *
 * The initialized item_size value cannot be modified after initialized.
 * The Initial capacity can be defined by STACK_INITIAL_CAPACITY macro.
 *
 * Usage:
 *   Include the header file in any file where you want to use the stack: #include "sh25_stack.h"
 *   Define the implementation once in your project: #define SH25_STACK_IMPLEMENTATION
 *    - Create a stack: Stack stack = {0};
 *    - Must call `stack_init` before any use.
 *    - Pop, drop and peek return STACK_EMPTY on an empty stack.
 *    - Call stack_destroy when done to free memory.
 *
 * Example usage:
 *   #define SH25_STACK_IMPLEMENTATION
 *   #include "sh25_stack.h"
 *   #include <stdio.h>
 *   Stack stack = {0};
 *   stack_init(stack, sizeof(int));
 *   int a = 10;
 *   stack_push(stack, a);
 *   int value;
 *   stack_peek(stack, value);
 *   printf("Top value: %d\n", value);
 *   stack_drop(stack);
 *   stack_destroy(stack);
 *
 * strong gurantee: if a function fails the stack is unchanged and still valid.
 *
 * Author: shalom2552
 * License: MIT
 */
#ifndef SH25_STACK_fe38ae3e35107192f10c318e1f600880
#define SH25_STACK_fe38ae3e35107192f10c318e1f600880

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STACK_OK,
    STACK_MEMORY_ERROR,
    STACK_EMPTY,
} StackResult;

typedef struct Stack Stack;

void sh25_stack_init(Stack* stack, size_t item_size);
StackResult sh25_stack_push(Stack* stack, void* item);
StackResult sh25_stack_pop(Stack* stack, void* pop);
StackResult sh25_stack_peek(Stack* stack, void* peek);
int sh25_stack_size(Stack* stack);
int sh25_stack_empty(Stack* stack);
void sh25_stack_clear(Stack* stack);
void sh25_stack_destroy(Stack* stack);

#define stack_init(stack, item_size)    sh25_stack_init(&stack, item_size)
// to copy the item to the list we need an lvalue, and to support rvalues we
// cast it to lvalue by creating the onject of whatever type it is.
// to convert to (void*)lvalue: (void*)(__typeof__(item)[1]){ (item) })
#define stack_push(stack, item)         sh25_stack_push(&stack, (void*)(__typeof__(item)[1]){ (item) })
#define stack_pop(stack, pop)           sh25_stack_pop(&stack, (void*)&pop)
#define stack_drop(stack)               sh25_stack_pop(&stack, NULL)
#define stack_peek(stack, peek)         sh25_stack_peek(&stack, (void*)&peek)
#define stack_size(stack)               sh25_stack_size(&stack)
#define stack_empty(stack)              sh25_stack_empty(&stack)
#define stack_clear(stack)              sh25_stack_clear(&stack)
#define stack_destroy(stack)            sh25_stack_destroy(&stack)

#ifdef SH25_STACK_IMPLEMENTATION

#ifndef STACK_INITIAL_CAPACITY
#define STACK_INITIAL_CAPACITY 16
#endif // !STACK_INITIAL_CAPACITY

struct Stack {
    void* top;
    void* data;
    size_t capacity;
    size_t item_size;
    size_t size;
};

void sh25_stack_init(Stack *stack, size_t item_size)
{
    assert(stack);

    stack->top = NULL;
    stack->data = NULL;
    stack->item_size = item_size;
    stack->capacity = 0;
    stack->size = 0;
}

StackResult sh25_stack_push(Stack* stack, void* item)
{
    assert(stack);
    assert(item);

    if (stack->size >= stack->capacity) {
        size_t capacity = stack->capacity == 0 ? STACK_INITIAL_CAPACITY : stack->capacity * 2;
        void* data = realloc(stack->data, capacity * stack->item_size);
        if (!data) {
            return STACK_MEMORY_ERROR;
        }

        stack->data = data;
        stack->capacity = capacity;
    }

    memcpy((char*)stack->data + stack->item_size * (stack->size), item, stack->item_size);
    stack->top = (void*)((char*)stack->data + stack->item_size * stack->size);

    ++stack->size;
    return STACK_OK;
}

StackResult sh25_stack_pop(Stack* stack, void* pop)
{
    if (stack->size == 0) {
        return STACK_EMPTY;
    }
    assert(stack);
    assert(stack->data && "Is stack initialized?");

    if (pop) {
        memcpy(pop, stack->top, stack->item_size);
    }
    --stack->size;

    if (stack->size < stack->capacity / 4) {
        size_t capacity = stack->capacity / 2;
        void* data = realloc(stack->data, capacity * stack->item_size);
        if (data) {
            stack->data = data;
            stack->capacity = capacity;
        }
    }

    stack->top = (void*)((char*)stack->data + stack->item_size * (stack->size - 1));
    return STACK_OK;
}

StackResult sh25_stack_peek(Stack* stack, void* peek)
{
    if (stack->size == 0) {
        return STACK_EMPTY;
    }

    assert(stack);
    assert(peek);
    assert(stack->data && "Is stack initialized?");

    memcpy(peek, stack->top, stack->item_size);
    return STACK_OK;
}

int sh25_stack_size(Stack* stack)
{
    assert(stack);

    return stack->size;
}

int sh25_stack_empty(Stack* stack)
{
    assert(stack);
    return stack->size == 0;
}

void sh25_stack_clear(Stack* stack)
{
    assert(stack);
    stack->top = (void*)((char*)stack->data + stack->item_size * stack->size);
    stack->size = 0;
}

void sh25_stack_destroy(Stack* stack)
{
    assert(stack);

    free(stack->data);

    stack->data = NULL;
    stack->top = NULL;
    stack->capacity = 0;
    stack->size = 0;
    stack = NULL;
}

#endif // SH25_STACK_IMPLEMENTATION

#endif // !SH25_STACK_fe38ae3e35107192f10c318e1f600880

