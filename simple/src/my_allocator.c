#include "my_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t raw_memory[TOTAL_SIZE];

static const uint8_t *end_addr = raw_memory + sizeof(uint8_t) * TOTAL_SIZE;

typedef struct MemInfo {
    size_t size;
    uint8_t *ptr;
    struct MemInfo *next;
} MemInfo;

MemInfo *head = (MemInfo*)raw_memory;

void init_allocator(void) {
    head->size = 0;
    head->ptr = raw_memory + sizeof(MemInfo);
    head->next = NULL;
}

void *my_malloc(const size_t size) {
    MemInfo *info = head;

    while ((uint8_t*)info < (end_addr - sizeof(MemInfo))) {
        if ((info->next == NULL) || (info->size == 0)) {
            if ((head->ptr + size) < end_addr) {
                info->size = size;
                head->next = (MemInfo*)(info->ptr + size);

                MemInfo *next = head->next;
                next->size = 0;
                next->ptr = (uint8_t*)next + sizeof(MemInfo);
                next->next = NULL;

                return (void*)info->ptr;
            }

            return NULL;
        }

        info = info->next;
    }

    return NULL;
}

void my_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    MemInfo *info = head;
    while ((uint8_t*)info < end_addr) {
        if ((void*)info->ptr == ptr) {
            info->size = 0;
            return;
        }

        info = info->next;
    }
}
