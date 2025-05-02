#include "../headers/deque.h"

#include <stdio.h>
#include <stdlib.h>



T
dq_empty(Deque *d){
    if (d == NULL) {
        fprintf(stderr, "dq_pop_back: deque is NULL\n");
        fflush(stderr);
        exit(1);
    }
    if (d->size == 0) {
        return 1;
    }
    return 0;
}


T
dq_pop_back(Deque *d) {
    if (dq_empty(d)) {
        fprintf(stderr, "dq_pop_back: deque is empty\n");
        fflush(stderr);
        exit(1);
    }
    T elem;
    elem = l2_get(d, d->size - 1);
    l2_erase(d, d->size - 1);
    return elem;
}

T
dq_pop_front(Deque *d) {
    if (dq_empty(d)) {
        fprintf(stderr, "dq_pop_from: deque is empty\n");
        fflush(stderr);
        exit(1);
    }
    T elem;
    elem = l2_get(d, 0);
    l2_erase(d, 0);
    return elem;

}

T
dq_back(Deque *d) {
    if (dq_empty(d)) {
        fprintf(stderr, "dq_back: deque is empty\n");
        fflush(stderr);
        exit(1);
    }
    return l2_get(d, d->size - 1);
}

T
dq_front(Deque *d) {
    if (dq_empty(d)) {
        fprintf(stderr, "dq_front: deque is empty\n");
        fflush(stderr);
        exit(1);
    }
    return l2_get(d, 0);
}

void
dq_push_front(Deque *d, T elem) {
    l2_push_front(d, elem);
}

void
dq_push_back(Deque *d, T elem) {
    l2_push_back(d, elem);
}
