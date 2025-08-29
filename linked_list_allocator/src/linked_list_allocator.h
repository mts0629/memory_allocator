#include <stddef.h>

// Heap size in bytes
#define HEAP_SIZE 200

// Initialize the allocator
void init_allocator(void);

// Allocate a specified size memory
void *mem_alloc(const size_t size);

// Free allocated memory
void mem_free(void *ptr);
