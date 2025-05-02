#ifndef HLIST_H
#define HLIST_H

#include <stdio.h>

typedef struct List {
    char *data;
    struct List *next;
} List;

extern void hl_printf(List *list);
extern List *hl_push_back(List *list, char *elem);
// extern List *l_insert(List *list, T elem, int pos);
// extern List *l_erase(List *list, int pos);
// extern List *l_update(List *list, T elem, int pos);
// extern T l_get(List *list, int pos);
// extern List *l_find(List *list, T elem);

#endif

