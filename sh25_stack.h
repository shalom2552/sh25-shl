/*
 * Shalome2552 stack single header file library.
 *
 * stack_init - must call init before use
 * stack_clear
 * stack_destroy
 * stack_push -
 * stack_pop - 
 * stack_peek
 * stack_size
 * stack_empty
 *
 */
#ifndef SH25_STACK_fe38ae3e35107192f10c318e1f600880
#define SH25_STACK_fe38ae3e35107192f10c318e1f600880

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define STACK_INITIAL_CAPACITY 16

typedef struct {
    void* top;
    void* data;
    size_t capacity;
    size_t item_size;
    size_t size;
} stack_t;

void sh25_stack_init(stack_t* stack, size_t item_size);
void sh25_stack_clear(stack_t* stack);
void sh25_stack_destroy(stack_t* stack);
void sh25_stack_push(stack_t* stack, void* item);
void sh25_stack_pop(stack_t* stack, void* pop);
void sh25_stack_peek(stack_t* stack, void* peek);
size_t sh25_stack_size(stack_t* stack);
int sh25_stack_empty(stack_t* stack);

#define stack_init(stack, item_size)    sh25_stack_init(&stack, item_size)
#define stack_clear(stack)              sh25_stack_clear(&stack)
#define stack_destroy(stack)            sh25_stack_destroy(&stack)
#define stack_push(stack, item)         sh25_stack_push(&stack, (void*)&item)
#define stack_pop(stack, pop)           sh25_stack_pop(&stack, (void*)&pop)
#define stack_peek(stack, peek)         sh25_stack_peek(&stack, (void*)&peek)
#define stack_size(stack)               sh25_stack_size(&stack)
#define stack_empty(stack)              sh25_stack_empty(&stack)


// // TODO: Remove this line
// #define SH25_STACK_IMPLEMENTATION
// // TODO: Remove this line


#ifdef SH25_STACK_IMPLEMENTATION

void sh25_stack_init(stack_t *stack, size_t item_size)
{
    assert(stack);

    stack->top = NULL;
    stack->data = malloc(item_size * STACK_INITIAL_CAPACITY);
    stack->capacity = STACK_INITIAL_CAPACITY;
    stack->item_size = item_size;
    stack->size = 0;

    // TODO: Checck this
    // if malloc fails, what to do?
    // should we make stack NULL?
    if (!stack->data) {
        return;
    }
}

// void sh25_stack_clear(stack_t* stack);

void sh25_stack_destroy(stack_t* stack)
{
    assert(stack);
    assert(stack->data);

    free(stack->data);

    stack->data = NULL;
    stack->top = NULL;
    stack->capacity = 0;
    stack->size = 0;
    stack->item_size = 0;
    stack = NULL;
}

void sh25_stack_push(stack_t* stack, void* item)
{
    assert(stack);
    assert(item);
    assert(stack->data);

    if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->data = realloc(stack->data, stack->capacity);
        assert(stack->data);
    }

    memcpy((char*)stack->data + stack->item_size * (stack->size), item, stack->item_size);
    stack->top = (void*)((char*)stack->data + stack->item_size * stack->size);
    ++stack->size;
}

void sh25_stack_pop(stack_t* stack, void* pop)
{
    assert(stack);
    assert(pop);

    if (stack->size == 0) {
        // TODO: handle empty pop
        return;
    }

    memcpy(pop, stack->top, stack->item_size);
    --stack->size;

    if (stack->size < stack->capacity / 4) {
        stack->capacity = stack->capacity / 2;
        stack->data = (void*)realloc(stack->data, stack->capacity);
        assert(stack->data);
    }

    stack->top = (void*)((char*)stack->data + stack->item_size * (stack->size - 1));
}

// void sh25_stack_peek(stack_t* stack, void* peek);
// void sh25_stack_size(stack_t* stack);
// void sh25_stack_empty(stack_t* stack);

#endif // SH25_STACK_IMPLEMENTATION

#endif // !SH25_STACK_fe38ae3e35107192f10c318e1f600880

