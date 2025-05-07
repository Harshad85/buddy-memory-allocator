#include "buddy_allocator.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MIN_ORDER 5                // 2^5 = 32 bytes
#define MAX_ORDER 20               // 2^20 = 1MB
#define HEAP_SIZE (1 << MAX_ORDER)
#define ORDERS (MAX_ORDER + 1)

typedef struct Block {
    struct Block *next;
    int order;
    int is_free;
} Block;

static char heap[HEAP_SIZE];
static Block *free_list[ORDERS];

static inline size_t block_size(int order) {
    return (1 << order);
}

static inline size_t align_up(size_t size) {
    size_t aligned = 1 << MIN_ORDER;
    while (aligned < size) aligned <<= 1;
    return aligned;
}

static inline int order_for_size(size_t size) {
    int order = MIN_ORDER;
    while ((1U << order) < size + sizeof(Block)) order++;
    return order;
}

static inline Block *get_buddy(Block *block) {
    uintptr_t offset = (uintptr_t)((char *)block - heap);
    uintptr_t buddy_offset = offset ^ (1 << block->order);
    return (Block *)(heap + buddy_offset);
}

void buddy_init() {
    memset(free_list, 0, sizeof(free_list));
    Block *initial = (Block *)heap;
    initial->order = MAX_ORDER;
    initial->is_free = 1;
    initial->next = NULL;
    free_list[MAX_ORDER] = initial;
}

void *buddy_malloc(size_t size) {
    int req_order = order_for_size(size);
    int order = req_order;
    while (order <= MAX_ORDER && !free_list[order]) {
        order++;
    }
    if (order > MAX_ORDER) return NULL;

    Block *block = free_list[order];
    free_list[order] = block->next;

    while (order > req_order) {
        order--;
        Block *buddy = (Block *)((char *)block + (1 << order));
        buddy->order = order;
        buddy->is_free = 1;
        buddy->next = free_list[order];
        free_list[order] = buddy;
        block->order = order;
    }

    block->is_free = 0;
    block->next = NULL;
    return (void *)(block + 1);
}

void buddy_free(void *ptr) {
    if (!ptr) return;
    Block *block = ((Block *)ptr) - 1;
    block->is_free = 1;

    while (block->order < MAX_ORDER) {
        Block *buddy = get_buddy(block);
        if (!buddy->is_free || buddy->order != block->order) break;

        // Remove buddy from free list
        Block **curr = &free_list[buddy->order];
        while (*curr && *curr != buddy) curr = &(*curr)->next;
        if (*curr) *curr = buddy->next;

        if ((char *)buddy < (char *)block) block = buddy;
        block->order++;
    }

    block->next = free_list[block->order];
    free_list[block->order] = block;
}

