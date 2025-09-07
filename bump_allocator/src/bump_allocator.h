#ifndef BUMP_ALLOCATOR_H
#define BUMP_ALLOCATOR_H

#include <stddef.h>

#ifndef HEAP_SIZE
// Heap size in bytes
#define HEAP_SIZE 1024
#endif

#ifdef __GNUC__
// Initialize the allocator
void init_allocator(void);
#endif

// Allocate a specified size memory
void *mem_alloc(const size_t size);

// Free allocated memory
void mem_free(void *ptr);

// Debug print
void debug_print(void);

#endif  // BUMP_ALLOCATOR_H
