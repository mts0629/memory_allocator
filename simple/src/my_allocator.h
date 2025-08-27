#include <stddef.h>

// Total size of memory block
#define TOTAL_SIZE 200

// Allocate a specified size memory
void *my_malloc(const size_t size);

// Free specified memory
void my_free(void *ptr);
