#include "buddy_allocator.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifndef NDEBUG
#include <stdio.h>
#endif

// Heap area
static uint8_t heap[BLOCK_SIZE * (ORDER + 1)];

// State of memory block
typedef enum State {
    FREE,
    ALLOCATED,
    SPLITTED
} State;

typedef struct Node {
    int order;
    uint8_t *addr;
    size_t size;
    State state;
    struct Node *left;
    struct Node *right;
} Node;

static int node_index = 0;
Node nodes[256];

static Node *head = nodes;

// Initialize a node
static void init_node(Node *node, const int order, uint8_t *addr, const size_t size) {
    node->order = order;
    node->addr = addr;
    node->size = size;
    node->state = FREE;
    node->left = NULL;
    node->right = NULL;
}

#ifdef __GNUC__
__attribute__((constructor))
#endif
void init_allocator(void) {
    static bool initialized = false;

    if (!initialized) {
        init_node(head, ORDER, heap, sizeof(heap));
        node_index++;

        initialized = true;
    }
}

// Get a memory order
static int get_order(const size_t size) {
    size_t num_blocks = ((size + BLOCK_SIZE - 1) / BLOCK_SIZE);

    int order = 0;
    while (pow(2, order) < num_blocks) {
        order++;
    }

    if (order > ORDER) {
        return -1;
    }

    return order;
}

// Search a free memory block
static uint8_t *search_free_block(Node *node, const int order) {
    if (node->order == order) {
        if (node->state == FREE) {
            node->state = ALLOCATED;
            return node->addr;
        }
    }

    if (node->left) {
        uint8_t *addr = search_free_block(node->left, order);
        if (addr) {
            return addr;
        }
    }

    if (node->right) {
        uint8_t *addr = search_free_block(node->right, order);
        if (addr) {
            return addr;
        }
    }

    return NULL;
}

// Split the heap
static bool split_heap(Node *node, const int order, const int cur_order) {
    if (cur_order == order) {
        if (node->state == FREE) {
            return true;
        }
    }

    node->state = SPLITTED;

    int next_order = cur_order - 1;
    size_t next_size = node->size / 2;

    if (node->left == NULL) {
        Node *new_node = &node[node_index];
        init_node(new_node, next_order, node->addr, next_size);
        node->left = new_node;

        node_index++;
    }
    if (split_heap(node->left, order, next_order)) {
        return true;
    }

    if (node->right == NULL) {
        Node *new_node = &node[node_index];
        init_node(new_node, next_order, (node->addr + next_size), next_size);
        node->right = new_node;

        node_index++;
    }
    if (split_heap(node->right, order, next_order)) {
        return true;
    }

    return false;
}

void *mem_alloc(const size_t size) {
    if (size > sizeof(heap)) {
        return NULL;
    }

    int order = get_order(size);
    if (order == -1) {
        return NULL;
    }

    uint8_t *addr = search_free_block(head, order);
    if (addr == NULL) {
        if (split_heap(head, order, ORDER)) {
            addr = search_free_block(head, order);
        }
    }

    return (void*)addr;
}

/*
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
*/

#ifdef NDEBUG
void debug_print(void) {}
#else
static void debug_print_node(const Node *node) {
    printf("order=%d, addr=%p, size=%lu\n", node->order, node->addr, node->size);
    if (node->left) {
        debug_print_node(node->left);
    }
    if (node->right) {
        debug_print_node(node->right);
    }
}

void debug_print(void) {
    debug_print_node(head);
}
#endif
