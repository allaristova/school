#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

typedef int T;

typedef struct Vector {
    T *data;
    int size;
    int capacity;
} Vector;

extern void v_push_back(Vector *vector, T elem);
extern void v_insert(Vector *vector, T elem, int pos);
extern void v_erase(Vector *vector, int pos);
extern void v_update(Vector *vector, T elem, int pos);
extern T v_get(Vector *vector, int pos);


#endif