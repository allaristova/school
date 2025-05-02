#ifndef DEQUE_H
#define DEQUE_H

#include "list.h"

typedef L2 Deque;

extern T dq_empty(Deque *d);
extern T dq_pop_back(Deque *d);
extern T dq_pop_front(Deque *d);
extern T dq_back(Deque *d);
extern T dq_front(Deque *d);
extern void dq_push_front(Deque *d, T elem);
extern void dq_push_back(Deque *d, T elem);

#endif