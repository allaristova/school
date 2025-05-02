#ifndef MAP_H
#define MAP_H

#include <stdio.h>

typedef struct List {
    char *value;
    struct List *next;
} List;

typedef struct CounterList {
    char *value;
    int count;
    struct CounterList *next;
} CounterList;

typedef struct BucketCell {
    char *key;
    List *values;
    struct BucketCell *next;
} BucketCell;

typedef struct UnorderedMap {
    unsigned int (*hash)(char *str);
    BucketCell *buckets[1 << 16];
} UnorderedMap;

extern UnorderedMap *map_create();

extern void map_printf(UnorderedMap *map);
extern void keys_printf(BucketCell *cell);
extern void list_printf(List *list);

extern void map_fprintf(UnorderedMap *map, FILE *f);
extern void keys_fprintf(BucketCell *cell, FILE *f);
extern void list_fprintf(List *list, FILE *f);

extern unsigned int hash_function(char *str);
extern void map_insert(UnorderedMap *map, char *key, char *value);
extern List *map_get(UnorderedMap *map, char *key);
extern CounterList *counter_create();
extern List *get_list_with_count(CounterList *ct, int count);
extern int counter_get(CounterList *ct, char *value);
extern void counter_insert_list(CounterList *ct, List *list);
extern void counter_insert(CounterList *ct, char *value);

#endif
