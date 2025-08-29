#include "linked_list_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint8_t heap[HEAP_SIZE];

static const uint8_t *heap_end = heap + sizeof(uint8_t) * HEAP_SIZE;

typedef struct MemInfo {
    size_t size;
    uint8_t *ptr;
    struct MemInfo *next;
} MemInfo;

MemInfo *head = (MemInfo*)heap;

void init_allocator(void) {
    head->size = 0;
    head->ptr = heap + sizeof(MemInfo);
    head->next = NULL;
}

void *mem_alloc(const size_t size) {
    MemInfo *info = head;

    while ((uint8_t*)info < (heap_end - sizeof(MemInfo))) {
        if ((info->next == NULL) || (info->size == 0)) {
            if ((head->ptr + size) < heap_end) {
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

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    MemInfo *info = head;
    while ((uint8_t*)info < heap_end) {
        if ((void*)info->ptr == ptr) {
            info->size = 0;
            return;
        }

        info = info->next;
    }
}
