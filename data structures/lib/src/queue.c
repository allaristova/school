#include "../headers/queue.h"

#include <stdio.h>
#include <stdlib.h>


T
q_dequeue(Queue *q) {
    return dq_pop_front(q);
}

void
q_enqueue(Queue *q, T elem) {
    dq_push_back(q, elem);
}

T
q_first(Queue *q) {
    return dq_front(q);
}

T
q_empty(Queue *q) {
    return dq_empty(q);
}

