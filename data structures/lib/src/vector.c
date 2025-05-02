#include "../headers/vector.h"

#include <stdlib.h>
#include <stdio.h>

void
v_push_back(Vector *vector, T elem) {  // '->' т к обращение к *
    if (vector->size + 1 <= vector->capacity) {
        vector->size++;
        vector->data[vector->size - 1] = elem;
    } else {
        int *new_data = realloc(vector->data, 2 * vector->capacity * sizeof(new_data));
        if (new_data == NULL) {
            exit(1); //выход из программы
        }
        // free(vector->data); - уже в realloc
        vector->data = new_data;
        vector->size++;
        vector->capacity *= 2;
        vector->data[vector->size - 1] = elem;
    }
    return;
}

void
v_insert(Vector *vector, T elem, int pos) {
    if (pos < 0 || pos >= vector->size) {
       printf("%s\n", "v_insert: invalid position");
       exit(1);
    }
    v_push_back(vector, elem);
    for (int i = pos; i < vector->size; ++i) {
        vector->data[i + 1] = vector->data[i];
    }
    vector->data[pos] = elem;
    return;
}

void
v_erase(Vector *vector, int pos) {
    if (pos < 0 || pos >= vector->size) {
       printf("%s\n", "v_erase: invalid position");
       exit(1);
    }
    for (int i = pos; i < vector->size - 1; i++) {
       vector->data[i] = vector->data[i + 1];
    }
    vector->size--;
    return;
}

void
v_update(Vector *vector, T elem, int pos) {
    if (pos < 0 || pos >= vector->size) {
       printf("%s\n", "v_update: invalid position");
       exit(1);
    }
    vector->data[pos] = elem;
    return;
}

int
v_get(Vector *vector, int pos) {
    if (pos < 0 || pos >= vector->size) {
       printf("%s\n", "v_get: invalid position");
       exit(1);
    }
    return vector->data[pos];
}


