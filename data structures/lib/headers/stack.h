#ifndef STACK_H
#define STACK_H

#include "deque.h"

typedef Deque Stack;

extern T st_pop(Stack *s);
extern void st_push(Stack *s, T elem);
extern T st_top(Stack *s);
extern T st_empty(Stack *s);

#endif
