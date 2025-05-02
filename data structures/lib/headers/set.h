#ifndef SET_H
#define SET_H

#include <stdio.h>

typedef struct List {
    char *data;
    struct List *next;
} List;

typedef struct UnorderedSet {
    unsigned int (*hash)(char *str);
    List *arr[1 << 16];
} UnorderedSet;

extern UnorderedSet *set_create();
extern void set_printf(UnorderedSet *set);
extern void set_insert(UnorderedSet *set, char *str);
extern void set_erase(UnorderedSet *set, char *str);
extern int set_find(UnorderedSet *set, char *str);
extern UnorderedSet *set_intersection(UnorderedSet *set1, UnorderedSet *set2);

#endif