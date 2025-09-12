#include "buddy_allocator.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifndef NDEBUG
#include <stdio.h>
#endif

// Heap area
static uint8_t heap[BLOCK_SIZE * (ORDER + 1)];

// Free node
typedef struct Node {
    size_t size;
    struct Node *next;
} Node;

// Head of the free list
static Node *head = (Node *)heap;

#ifdef __GNUC__
__attribute__((constructor))
#endif
void init_allocator(void) {
    static bool initialized = false;

    if (!initialized) {
        head->size = sizeof(heap);
        head->next = NULL;

        // Initialize the allocator only once
        initialized = true;
    }
}

static inline uint8_t *mem_start_address(const Node *node) {
    return (uint8_t *)node + sizeof(Node);
}

void *mem_alloc(const size_t size) {
    int order = ORDER;
    size_t block_size = sizeof(heap);

    if (size > block_size) {
        return NULL;
    }

    while (order > 0) {
        block_size /= 2;

        if (block_size <= size) {
            block_size *= 2;
            break;
        }

        order--;
    }

    return heap;
}

static inline Node *node_address(const void *ptr) {
    return (Node *)((uint8_t *)ptr - sizeof(Node));
}

static inline Node *next_node_address(const Node *node) {
    return (Node *)(mem_start_address(node) + node->size);
}

void mem_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    Node *free_node = node_address(ptr);

    Node *node = head;
    while (node != NULL) {
        // If deallocated node is next to the existing one, combine them
        if (next_node_address(node) == free_node) {
            node->size += (sizeof(Node) + free_node->size);
            return;
        }

        node = node->next;
    }

    // Attach a deallocated memory into the head of the free list
    Node *current_head = head;
    head = free_node;
    head->next = current_head;
}

#ifdef NDEBUG
void debug_print(void) {}
#else
void debug_print(void) {
    Node *node = head;
    int i = 0;
    while (node != NULL) {
        printf("[%d] size=%lu ", i, node->size);
        node = node->next;
        i++;
    }
    putchar('\n');
}
#endif
