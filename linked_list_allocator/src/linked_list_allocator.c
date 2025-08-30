#include "linked_list_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>

static uint8_t heap[HEAP_SIZE];

typedef struct MemInfo {
    size_t size;
    struct MemInfo *next;
} MemInfo;

static MemInfo *free_head = (MemInfo*)heap;

void init_allocator(void) {
    free_head->size = sizeof(heap) - sizeof(MemInfo);
    free_head->next = (MemInfo*)(heap + sizeof(MemInfo));
}

void *mem_alloc(const size_t size) {
    MemInfo *info = free_head;

    while (true) {
        if (size <= info->size) {
            if ((info->size - size) < sizeof(MemInfo)) {
                return NULL;
            }

            void *ptr = (void*)info->next;

            size_t current_size = info->size;

            info->size = size;
            info->next = NULL;

            free_head = (MemInfo*)(info + size);
            free_head->size = current_size - size - sizeof(MemInfo);
            free_head->next = free_head + sizeof(MemInfo);

            return ptr;
        }

        if (info->next == info) {
            break;
        }

        info = info->next;
    }

    return NULL;
}

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    MemInfo *info = (MemInfo*)((uint8_t*)ptr - sizeof(MemInfo));

    MemInfo *current_head = free_head;
    free_head = info;
    free_head->next = current_head;
}
