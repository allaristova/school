#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../lib/headers/tree.h"
#include "../../lib/headers/avl.h"


int
main(void) {
    Heap *heap = heap_create(1);
    // heap_printf(heap);
    // heap_insert(heap, 2);
    // heap_printf(heap);
    // heap_insert(heap, 5);
    // heap_printf(heap);
    // heap_insert(heap, 4);
    // heap_printf(heap);
    // heap_insert(heap, 7);
    // heap_printf(heap);
    // heap_insert(heap, 13);
    // heap_printf(heap);
    // heap_insert(heap, 10);
    // heap_printf(heap);
    // printf("%d %d %d\n", heap->root->data, heap->root->left->data, heap->root->right->data);
    // printf("%d\n", heap->root->right->left->data);
    // printf("%d\n", heap->root->left->left->data);
    bst_insert(heap, 2);
    heap_printf(heap);
    bst_insert(heap, 5);
    heap_printf(heap);
    bst_insert(heap, 4);
    heap_printf(heap);
    bst_insert(heap, 7);
    heap_printf(heap);
    bst_erase(heap, 5);
    heap_printf(heap);
    printf("%d\n", bst_find(heap, 7));
    printf("%d\n", bst_find(heap, 2));
    printf("%d\n", bst_find(heap, 5));
    printf("%d\n", bst_find(heap, 13));
    Vector *vector = bst_to_vector(heap);
    for (int i = 0; i < vector->size; ++i) {
        printf("%d ", vector->data[i]);
    }
    printf("\n");
    // printf("%d\n", heap->root->data);
    // printf("%d %d\n", heap->root->data, heap->root->right->data);
   // heap_erase_root(heap);
    //heap_printf(heap);

    return 0;
}
