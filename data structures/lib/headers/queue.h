#ifndef QUEUE_H
#define QUEUE_H

#include "deque.h"

typedef Deque Queue;

extern T q_dequeue(Queue *q);
extern void q_enqueue(Queue *q, T elem);
extern T q_first(Queue *q);
extern T q_empty(Queue *q);

#endif