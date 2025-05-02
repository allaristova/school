#include "../headers/stack.h"

#include <stdio.h>
#include <stdlib.h>


T
st_pop(Stack *s) {
    return dq_pop_back(s);
}

void
st_push(Stack *s, T elem) {
    dq_push_back(s, elem);
}

T
st_top(Stack *s) {
    return dq_back(s);
}

T
st_empty(Stack *s){
    return dq_empty(s);
}