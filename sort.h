#ifndef SORT_H
#define SORT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Tells you which arg is bigger. -1 for first bigger, 0 if equal, 1 if last bigger 
typedef char (*ComparisonFunc)(const void*, const void*);

void bubble_sort_single_pass(void* buf, size_t count, size_t elem_size, ComparisonFunc comparison_func);
void bubble_sort(void* buf, size_t count, size_t elem_size, ComparisonFunc comparison_func);

#endif

#ifdef SORT_IMPLEMENTATION

void bubble_sort_single_pass(void* buf, size_t count, size_t elem_size, ComparisonFunc comparison_func) {
   
    uint8_t* start = (uint8_t*)buf;
    uint8_t* end   = start + (count * elem_size);
    
    void* tmp = malloc(elem_size);
    for ( ; start < end - elem_size; start += elem_size) {
        // Compare, swap if needed
        char result = comparison_func(start, start+elem_size);
        if (result == -1) {
            memcpy(tmp, start, elem_size);
            memcpy(start, start+elem_size, elem_size);
            memcpy(start+elem_size, tmp, elem_size);
        }
    }

    free(tmp);
}

void bubble_sort(void* buf, size_t count, size_t elem_size, ComparisonFunc comparison_func) {
    
    bool swapped = true;
    void* tmp = malloc(elem_size);

    // Buffer is unsorted as long as swaps happen
    while (swapped) {
        swapped = false;
        uint8_t* start = (uint8_t*)buf;
        uint8_t* end   = start + (count * elem_size);

        for ( ; start < end - elem_size; start += elem_size) {
            // Compare, swap if needed
            char result = comparison_func(start, start+elem_size);
            if (result == -1) {
                memcpy(tmp, start, elem_size);
                memcpy(start, start+elem_size, elem_size);
                memcpy(start+elem_size, tmp, elem_size);
                swapped = true;
            }
        }
    }
    free(tmp);
}

#endif
