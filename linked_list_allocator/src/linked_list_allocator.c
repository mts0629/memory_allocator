#include "linked_list_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Heap area
static uint8_t heap[HEAP_SIZE];

// Memory header
typedef struct MemInfo {
    size_t size;
    struct MemInfo *next;
} MemInfo;

// Head of the free list
static MemInfo *free_head = (MemInfo *)heap;

void init_allocator(void) {
    free_head->size = sizeof(heap);
    free_head->next = NULL;
}

void *mem_alloc(const size_t size) {
    MemInfo *info = free_head;

    while (info != NULL) {
        if (size <= info->size) {
            // Check the remaining are can store a memory header
            if ((info->size - size) < sizeof(MemInfo)) {
                return NULL;
            }

            uint8_t *ptr = (uint8_t *)info;

            // Attach the remaining area to the head of the free list
            MemInfo *prev_next = free_head->next;
            free_head = (MemInfo *)(ptr + sizeof(MemInfo) + size);
            free_head->size = info->size - sizeof(MemInfo) - size;
            free_head->next = prev_next;

            info->size = size;
            info->next = NULL;

            return (void *)(ptr + sizeof(MemInfo));
        }

        info = info->next;
    }

    return NULL;
}

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    // Attach a deallocated memory into the head of the free list
    MemInfo *current_head = free_head;

    MemInfo *info = (MemInfo *)((uint8_t *)ptr - sizeof(MemInfo));
    free_head = info;
    free_head->next = current_head;
}
