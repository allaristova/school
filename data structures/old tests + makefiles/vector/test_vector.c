#include <stdio.h>
#include <stdlib.h>

#include "../../lib/headers/vector.h"


int
main(void){
    int n;
    scanf("%d", &n);
    int *arr = calloc(n, sizeof(*arr));
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }
    Vector vector = {arr, n, n};
    v_push_back(&vector, 777);
    for (int i = 0; i < vector.size; ++i) {
        printf("%d ", vector.data[i]);
    }
    printf("\n");
    return 0;
}
