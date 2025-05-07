// buddy_allocator.h
#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <stddef.h>

void buddy_init();
void *buddy_malloc(size_t size);
void buddy_free(void *ptr);

#endif // BUDDY_ALLOCATOR_H

