#include <stdio.h>

#include "my_allocator.h"

int main(void) {
    init_allocator();

    // Allocate a memory
    int *arr1 = my_malloc(sizeof(int) * 10);
    printf("arr1 (%p): ", (void *)arr1);
    for (int i = 0; i < 10; i++) {
        arr1[i] = i;
        printf("%d ", arr1[i]);
    }
    putchar('\n');

    // Allocate an another memory
    int *arr2 = my_malloc(sizeof(int) * 10);
    printf("arr2 (%p): ", (void *)arr2);
    for (int i = 0; i < 10; i++) {
        arr2[i] = 2 * i;
        printf("%d ", arr2[i]);
    }
    putchar('\n');

    // Free the first one
    my_free(arr1);

    // Allocate another one
    int *arr3 = my_malloc(sizeof(int) * 5);
    printf("arr3 (%p): ", (void *)arr3);
    for (int i = 0; i < 5; i++) {
        arr3[i] = 3 * i;
        printf("%d ", arr3[i]);
    }
    putchar('\n');

    // Fail: try to allocate too much memory
    int *arr4 = my_malloc(sizeof(int) * 1000);
    printf("arr4 (%p)\n", (void *)arr4);

    return 0;
}
