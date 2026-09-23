/*
 * sh25_array.h - single-header array library for c.
 *
 * This library provides a simple dynamic array with dynamic memory allocation.
 *
 * Define SH25_ARRAY_IMPL in exactly ONE translation unit before including this header.
 *
 * API:
 *   array_init(array, item_size)   Initialize the array with a given item size, must call before use.
 *   array_alloc(array, size)       Allocate memory for the array with a given size. returns ARRAY_MEMORY_ERROR on memory fail.
 *   array_append(array, item)      Append an item to the array. returns ARRAY_MEMORY_ERROR on memory fail.
 *   array_get(array, index, get)   Get an element from the array by index. returns ARRAY_OUT_OF_BOUND on invalid index.
 *   array_set(array, index, set)   Set an element in the array by index. returns ARRAY_OUT_OF_BOUND on invalid index.
 *   array_size(array)              Returns the size of the array.
 *   array_empty(array)             Returns 1 if the array is empty, 0 otherwise.
 *   array_clear(array)             Clears the array and keeps the allocated capacity.
 *   array_destroy(array)           Destroys the array and frees the allocated memory.
 *
 * The initialized item_size value cannot be modified after initialized.
 * The Initial capacity can be defined by ARRAY_INITIAL_CAPACITY macro.
 *
 * Return values:
 *   ARRAY_OK: Operation successful.
 *   ARRAY_MEMORY_ERROR: Memory allocation failed.
 *   ARRAY_OUT_OF_BOUND: Index out of bounds.
 *
 * Usage:
 *   Include the header file in any file where you want to use the array: #include "sh25_array.h"
 *   Define the implementation once in your project: #define SH25_ARRAY_IMPL
 *    - Create a stack: Array array = {0};
 *    - Must call `array_init` before any use.
 *    - Call array_destroy when done to free memory.
 *
 * Example usage:
 *   #define SH25_ARRAY_IMPL
 *   #include "sh25_array.h"
 *   #include <stdio.h>
 *   Array array = {0};
 *   array_init(array, sizeof(int));
 *   array_append(array, 10);
 *   array_append(array, 20);
 *   int value;
 *   ArrayResult result = array_get(array, 1, value);
 *   printf("Value at index 1: %d\n", value);
 *   array_destroy(array);
 *
 * Author: shalom2552
 * License: MIT
 */
#ifndef SH25_ARRAY_H_35863394ccdccb4f186c6a7cedc68f06
#define SH25_ARRAY_H_35863394ccdccb4f186c6a7cedc68f06

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <string.h>

typedef enum {
    ARRAY_OK,
    ARRAY_MEMORY_ERROR,
    ARRAY_OUT_OF_BOUND,
} ArrayResult;

typedef struct {
    void* data;
    size_t capacity;
    size_t item_size;
    size_t size;
} Array;

void sh25_array_init(Array* array, size_t item_size);
ArrayResult sh25_array_alloc(Array* array, size_t size);
ArrayResult sh25_array_append(Array* array, void* item);
ArrayResult sh25_array_get(Array* array, size_t index, void* get);
ArrayResult sh25_array_set(Array* array, size_t index, const void* set);
size_t sh25_array_size(Array* array);
int sh25_array_empty(Array* array);
void sh25_array_clear(Array* array);
void sh25_array_destroy(Array* array);

#define array_init(array, item_size)    sh25_array_init(&array, item_size)
#define array_alloc(array, size)        sh25_array_alloc(&array, size)
// to copy the item to the list we need an lvalue, and to support rvalues we
// cast it to lvalue by creating the onject of whatever type it is.
// to convert to (void*)lvalue: (void*)(__typeof__(item)[1]){ (item) })
#define array_append(array, item)       sh25_array_append(&array, (void*)(__typeof__(item)[1]){ (item) })
#define array_get(array, index, get)    sh25_array_get(&array, index, (void*)&get)
#define array_set(array, index, set)    sh25_array_set(&array, index, (void*)&set)
#define array_size(array)               sh25_array_size(&array)
#define array_empty(array)              sh25_array_empty(&array)
#define array_clear(array)              sh25_array_clear(&array)
#define array_destroy(array)            sh25_array_destroy(&array)

#ifdef SH25_ARRAY_IMPL

#ifndef ARRAY_INITIAL_CAPACITY
#define ARRAY_INITIAL_CAPACITY 16
#endif // !ARRAY_INITIAL_CAPACITY

void sh25_array_init(Array *array, size_t item_size)
{
    assert(array);

    array->data = NULL;
    array->item_size = item_size;
    array->capacity = 0;
    array->size = 0;
}

ArrayResult sh25_array_alloc(Array* array, size_t size)
{
    assert(array);

    if (array->capacity >= size) {
        return ARRAY_OK;
    }

    void* data = realloc(array->data, size * array->item_size);
    if (!data) {
        return ARRAY_MEMORY_ERROR;
    }

    array->data = data;
    array->capacity = size;
    array->size = size;

    memset(array->data, 0, size * array->item_size);

    return ARRAY_OK;
}

ArrayResult sh25_array_append(Array* array, void* item)
{
    assert(array);
    assert(item);

    if (array->size >= array->capacity) {
        size_t capacity = array->capacity == 0 ? ARRAY_INITIAL_CAPACITY : array->capacity * 2;
        void* data = realloc(array->data, capacity * array->item_size);
        if (!data) {
            return ARRAY_MEMORY_ERROR;
        }

        array->data = data;
        array->capacity = capacity;
    }

    memcpy((char*)array->data + array->item_size * (array->size), item, array->item_size);

    ++array->size;
    return ARRAY_OK;
}

ArrayResult sh25_array_get(Array* array, size_t index, void* get)
{
    assert(array);

    if (index >= array->size) {
        return ARRAY_OUT_OF_BOUND;
    }

    assert(get);
    assert(array->data && "Is array initialized?");

    memcpy(get, (char*)array->data + index * array->item_size, array->item_size);

    return ARRAY_OK;
}

ArrayResult sh25_array_set(Array* array, size_t index, const void* set)
{
    assert(array);

    if (index >= array->size) {
        return ARRAY_OUT_OF_BOUND;
    }

    assert(set);
    assert(array->data && "Is array initialized?");

    memcpy((char*)array->data + index * array->item_size, set, array->item_size);

    return ARRAY_OK;
}

size_t sh25_array_size(Array* array)
{
    assert(array);

    return array->size;
}

int sh25_array_empty(Array* array)
{
    assert(array);

    return array->size == 0;
}

void sh25_array_clear(Array* array)
{
    assert(array);

    memset(array->data, 0, array->capacity * array->item_size);
    array->size = 0;
}

void sh25_array_destroy(Array* array)
{
    assert(array);

    free(array->data);

    array->data = NULL;
    array->capacity = 0;
    array->size = 0;
    array = NULL;
}

#endif // SH25_ARRAY_IMPL

#endif // !SH25_ARRAY_H_35863394ccdccb4f186c6a7cedc68f06

