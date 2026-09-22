#include <stddef.h>
#include <stdio.h>
#define SH25_ARRAY_IMPLEMENTATION
#include "../include/sh25_array.h"

void merge(Array* array, size_t left, size_t mid, size_t right) {

    size_t nr = right - mid;
    size_t nl = mid - left + 1;

    Array left_arr = {0};
    array_init(left_arr, sizeof(int));
    array_alloc(left_arr, nl);

    Array right_arr = {0};
    array_init(right_arr, sizeof(int));
    array_alloc(right_arr, nr);

    for (size_t i = 0; i < nl; ++i) {
        int value;
        array_get(*array, left + i, value);
        array_set(left_arr, i, value);
    }
    for (size_t j = 0; j < nr; ++j) {
        int value;
        array_get(*array, mid + 1 + j, value);
        array_set(right_arr, j, value);
    }

    size_t i = 0;
    size_t j = 0;
    size_t k = left;
    while (i < nl && j < nr) {
        int left_val, right_val;
        array_get(left_arr, i, left_val);
        array_get(right_arr, j, right_val);
        if (left_val <= right_val) {
            array_set(*array, k, left_val);
            ++i;
        } else {
            array_set(*array, k, right_val);
            ++j;
        }
        ++k;
    }

   while (i < nl) {
        int val; array_get(left_arr, i, val);
        array_set(*array, k, val);
        ++i;
        ++k;
    }

   while (j < nr) {
        int val; array_get(right_arr, j, val);
        array_set(*array, k, val);
        ++j;
        ++k;
    }

    array_destroy(left_arr);
    array_destroy(right_arr);
}

void merge_sort(Array* array, size_t left, size_t right)
{
    if (left < right) {
        int mid = left + (right - left) / 2;

        merge_sort(array, left, mid);
        merge_sort(array, mid + 1, right);

        merge(array, left, mid, right);
    }
}

#ifndef NO_MAIN
int main(void)
{
    Array array = {0};
    array_init(array, sizeof(int));

    array_append(array, 38);
    array_append(array, 27);
    array_append(array, 43);
    array_append(array, 10);

    merge_sort(&array, 0, array_size(array) - 1);

    int val;
    for (size_t i = 0; i < array_size(array); ++i) {
        array_get(array, i, val);
        printf("%d ", val);
    }
    printf("\n");

    array_destroy(array);
    return 0;
}
#endif // NO_MAIN

