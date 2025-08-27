#include "my_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t raw_memory[TOTAL_SIZE];

const uintptr_t last_addr =
    (uintptr_t)raw_memory + sizeof(uint8_t) * TOTAL_SIZE - 1;

typedef struct {
    uintptr_t addr;
    size_t size;
} MemInfo;

static MemInfo meminfo[TOTAL_SIZE];

void *my_malloc(const size_t size) {
    if (size > TOTAL_SIZE) {
        return NULL;
    }

    uintptr_t free_addr = (uintptr_t)raw_memory;
    int i = 0;
    int last_idx = 0;
    for (i = 0; i < TOTAL_SIZE; i++) {
        if (meminfo[i].size > 0) {
            free_addr = meminfo[i].addr + meminfo[i].size;
            last_idx = i;
        }
    }

    if ((free_addr + size) > last_addr) {
        return NULL;
    }

    meminfo[last_idx].addr = free_addr;
    meminfo[last_idx].size = size;

    return (void *)meminfo[last_idx].addr;
}

void my_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    for (int i = 0; i < TOTAL_SIZE; i++) {
        if ((void *)meminfo[i].addr == ptr) {
            meminfo[i].addr = 0x0;
            meminfo[i].size = 0;
        }
    }
}
