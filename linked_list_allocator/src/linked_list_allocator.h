#ifndef LINKED_LIST_ALLOCATOR_H
#define LINKED_LIST_ALLOCATOR_H

#include <stddef.h>

#ifndef HEAP_SIZE
// Heap size in bytes
#define HEAP_SIZE 1024
#endif

#ifndef __GNUC__
// Initialize the allocator
void init_allocator(void);
#endif

// Allocate a specified size memory
void *mem_alloc(const size_t size);

// Free allocated memory
void mem_free(void *ptr);

// Debug print for the free list
void debug_print(void);

#endif  // LINKED_LIST_ALLOCATOR_H
