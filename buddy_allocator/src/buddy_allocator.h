#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <stddef.h>

#ifndef BLOCK_SIZE
// Block size in bytes
#define BLOCK_SIZE 32
#endif
#ifndef ORDER
// Order
#define ORDER 4
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

#endif  // BUDDY_ALLOCATOR_H
