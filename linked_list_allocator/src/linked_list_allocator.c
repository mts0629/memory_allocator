#include "linked_list_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Heap area
static uint8_t heap[HEAP_SIZE];

// Free node
typedef struct Node {
    size_t size;
    struct Node *next;
} Node;

// Head of the free list
static Node *head = (Node *)heap;

void init_allocator(void) {
    static bool initialized = false;

    if (!initialized) {
        head->size = sizeof(heap);
        head->next = NULL;

        // Initialize the allocator only once
        initialized = true;
    }
}

void *mem_alloc(const size_t size) {
    Node *node = head;

    while (node != NULL) {
        if (size <= node->size) {
            // Check the remaining area can store a memory node
            if ((node->size - size) < sizeof(Node)) {
                return NULL;
            }

            uint8_t *ptr = (uint8_t *)node;

            // Attach the remaining area to the head of the free list
            Node *prev_next = head->next;
            head = (Node *)(ptr + sizeof(Node) + size);
            head->size = node->size - sizeof(Node) - size;
            head->next = prev_next;

            node->size = size;
            node->next = NULL;

            return (void *)(ptr + sizeof(Node));
        }

        node = node->next;
    }

    return NULL;
}

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    // Attach a deallocated memory into the head of the free list
    Node *current_head = head;

    Node *node = (Node *)((uint8_t *)ptr - sizeof(Node));
    head = node;
    head->next = current_head;
}
