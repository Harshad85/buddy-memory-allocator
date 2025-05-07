#include <stdio.h>
#include <string.h>
#include "buddy_allocator.h"

int main() {
    buddy_init();

    printf("Allocating 100 bytes\n");
    void *p1 = buddy_malloc(100);
    printf("Allocated at %p\n", p1);

    printf("Allocating 2000 bytes\n");
    void *p2 = buddy_malloc(2000);
    printf("Allocated at %p\n", p2);

    printf("Freeing first block\n");
    buddy_free(p1);

    printf("Freeing second block\n");
    buddy_free(p2);

    return 0;
}
