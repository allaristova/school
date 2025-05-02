#ifndef MAP_H
#define MAP_H

#include <stdio.h>

typedef struct List {
    char *value;
    struct List *next;
} List;

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
extern unsigned int hash_function(char *str);
extern void map_insert(UnorderedMap *map, char *key, char *value);
extern List *map_get(UnorderedMap *map, char *key);

#endif