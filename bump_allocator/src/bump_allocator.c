#include "bump_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifndef NDEBUG
#include <stdio.h>
#endif

// Heap area
static uint8_t heap[HEAP_SIZE];

// End address of the heap
static uint8_t *end_addr = heap + HEAP_SIZE - 1;

// Current pointer
static uint8_t *cur_ptr = heap;

// Current memory count
static size_t mem_count = 0;

void *mem_alloc(const size_t size) {
    if (size > HEAP_SIZE) {
        return NULL;
    }

    if ((cur_ptr + size) > (end_addr + 1)) {
        return NULL;
    }

    void *ptr = (void*)cur_ptr;
    cur_ptr += size;
    mem_count++;

    return ptr;
}

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    if (((uint8_t*)ptr < heap) || ((uint8_t*)ptr > end_addr)) {
        return;
    }

    if (mem_count > 0) {
        mem_count--;

        if (mem_count == 0) {
            cur_ptr = heap;
        }
    }
}

#ifdef NDEBUG
void debug_print(void) {}
#else
void debug_print(void) {
    printf("cur_ptr=%p, mem_count==%lu\n", cur_ptr, mem_count);
}
#endif
