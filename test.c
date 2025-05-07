#include <stdio.h>
#include <string.h>
#include "buddy_allocator.h"

typedef struct {
    int id;
    char name[32];
    float score;
} Student;

int main() {
    buddy_init();

    // Allocate integer array
    int *arr = (int *)buddy_malloc(10 * sizeof(int));
    if (arr) {
        for (int i = 0; i < 10; i++) arr[i] = i * i;
        printf("Integer array:\n");
        for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
        printf("\n");
    }

    // Allocate and copy string
    char *msg = (char *)buddy_malloc(50);
    if (msg) {
        strcpy(msg, "Buddy allocator works!");
        printf("String: %s\n", msg);
    }

    // Allocate structure
    Student *s = (Student *)buddy_malloc(sizeof(Student));
    if (s) {
        s->id = 101;
        strcpy(s->name, "Alice");
        s->score = 95.2;
        printf("Student: ID=%d Name=%s Score=%.1f\n", s->id, s->name, s->score);
    }

    // Free memory
    buddy_free(arr);
    buddy_free(msg);
    buddy_free(s);

    return 0;
}

