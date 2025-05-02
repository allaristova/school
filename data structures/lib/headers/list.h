#ifndef LIST_H
#define LIST_H

#include <stdio.h>

typedef char T;

typedef struct List {
    T data;
    struct List *next;
} List;

typedef struct L2Node {
    T data;
    struct L2Node *next;
    struct L2Node *prev;
} L2Node;

typedef struct L2 {
    L2Node *head;
    L2Node *tail;
    size_t size;
} L2;


extern void l_printf(List *list);
extern List *l_push_back(List *list, T elem);
extern List *l_insert(List *list, T elem, int pos);
extern List *l_erase(List *list, int pos);
extern List *l_update(List *list, T elem, int pos);
extern T l_get(List *list, int pos);
extern List *l_find(List *list, T elem);
extern List *l_remove_duplicates(List *list);

extern void l2_printf(L2 *list);
extern void l2_push_back(L2 *list, T elem);
extern void l2_push_front(L2 *list, T elem);
extern void l2_insert(L2 *list, T elem, int pos);
extern void l2_erase(L2 *list, int pos);
extern void l2_update(L2 *list, T elem, int pos);
extern T l2_get(L2 *list, int pos);
extern L2Node *l2_find(L2 *list, T elem);

#endif