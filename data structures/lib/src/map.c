#include "../headers/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned short int
MurmurHash2(char *key, unsigned int len) {
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char *data = (const unsigned char *)key;
    unsigned int k = 0;

    while(len >= 4)
    {
        k = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch(len)
    {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

unsigned int
hash_function(char *str) {
    return MurmurHash2(str, strlen(str));
}


UnorderedMap *
map_create() {
    UnorderedMap *map = (UnorderedMap *)calloc(1, sizeof(UnorderedMap));
    map->hash = &hash_function;
    return map;
}

void
list_printf(List *list) {
    List *current = list;
    while(current != NULL)
    {
        if(current->value != NULL)
            printf("%s ", current->value);
        current = current->next;
    }
    printf("\n");
}

void
keys_printf(BucketCell *cell) {
    BucketCell *current = cell;
    while(current != NULL)
    {
        //printf("Key %s: ", current->key);
        printf("%s: ", current->key);
        list_printf(current->values);
        //printf("\n");
        current = current->next;
    }
}

void
map_printf(UnorderedMap *map) {
    for(int i = 0; i < (1 << 16); i++) {
        if(map->buckets[i] != NULL) {
            //printf("Hash %d:\n", i);
            keys_printf(map->buckets[i]);
        }
    }
}

void
list_push_back(List *list, char *value) {
    List *current = list;
    while(current->next != NULL)
        current = current->next;
    current->next = (List *)calloc(1, sizeof(List));
    current->value = value;
}


void
map_insert(UnorderedMap *map, char *key, char *value) {
    unsigned short int ind = map->hash(key);
    BucketCell *cell = map->buckets[ind];
    if(cell == NULL) // if no cells for current key
    {
        map->buckets[ind] = (BucketCell *)calloc(1, sizeof(BucketCell));
        map->buckets[ind]->key = key;
        map->buckets[ind]->values = (List *)calloc(1, sizeof(List));
        cell = map->buckets[ind];
    }
    BucketCell *current = cell;
    while(strcmp(current->key, key) != 0)
    {
        if(current->next == NULL)
            break;
        current = current->next;
    }
    if(current->next == NULL && strcmp(current->key, key) != 0) // no cell for such key, so create>
    {
        current->next = (BucketCell *)calloc(1, sizeof(BucketCell));
        current = current->next;
        current->key = key;
        current->values = (List *)calloc(1, sizeof(List));
    }
    List *currentValues = current->values;
    list_push_back(currentValues, value);
}

List *
map_get(UnorderedMap *map, char *key) {
    unsigned int ind = map->hash(key);
    BucketCell *cell = map->buckets[ind];
    if(cell == NULL)
    { // no such key
        return NULL;
    }
    BucketCell *current = cell;
    while(current != NULL)
    {
        if(strcmp(current->key, key) == 0)
        {
            return current->values;
        }
        current = current->next;
    }
    return NULL; // no such key
}
