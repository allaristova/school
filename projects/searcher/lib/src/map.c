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

    while(len >= 4) {
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

    switch(len) {
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
    while(current != NULL) {
        if(current->value != NULL) {
            printf("%s ", current->value);
        }
        current = current->next;
    }
    printf("\n");
}

void
keys_printf(BucketCell *cell) {
    BucketCell *current = cell;
    while(current != NULL) {
        // printf("Key %s: ", current->key);
        printf("%s: ", current->key);
        list_printf(current->values);
        // printf("\n");
        current = current->next;
    }
}

void
counter_insert(CounterList *ct, char *value) {
    CounterList *current = ct;
    if(current->count == 0) { // new list
        current->value = (char *)calloc(sizeof(value), sizeof(char *));
        strcpy(current->value, value);
        current->count = 1;
        return;
    }
    while(strcmp(value, current->value) != 0 && current->next != NULL) { // find value
        current = current->next;
    }
    if(strcmp(value, current->value) == 0) {
        current->count++;
    }
    else { // no value in list yet
        current->next = (CounterList *)calloc(1, sizeof(CounterList));
        current = current->next;
        current->value = (char *)calloc(sizeof(value), sizeof(char *));
        strcpy(current->value, value);
        current->count = 1;
    }
}

void
counter_insert_list(CounterList *ct, List *list) { // insert whole list in counter
    List *current = list;
    while(current != NULL) {
        counter_insert(ct, current->value);
        current = current->next;
    }
}

int
counter_get(CounterList *ct, char *value) {
    CounterList *current = ct;
    while(strcmp(value, current->value) != 0 && current->next != NULL) {
        current = current->next;
    }
    if(strcmp(value, current->value) == 0) {
        return current->count;
    }
    return 0; // no such value
}

List *
get_list_with_count(CounterList *ct, int count) {
    List *list = (List *)calloc(1, sizeof(List));
    List *current = list;
    CounterList *current_counter = ct;
    while(current_counter != NULL) {
        if(current_counter->count == count) {
            if(current->value != NULL) { // if not first cell
                current->next = (List *)calloc(1, sizeof(List));
                current = current->next;
            }
            current->value = (char *)calloc(sizeof(current_counter->value), sizeof(char *));
            strcpy(current->value, current_counter->value);
        }
        current_counter = current_counter->next;
    }
    return list;
}

CounterList *
counter_create() {
    CounterList *ct = (CounterList *)calloc(1, sizeof(CounterList));
    ct->next = NULL;
    ct->count = 0;
    return ct;
}

void
map_printf(UnorderedMap *map) {
    for(int i = 0; i < (1 << 16); i++) {
        if(map->buckets[i] != NULL) {
            // printf("Hash %d:\n", i);
            keys_printf(map->buckets[i]);
        }
    }
}

void
list_fprintf(List *list, FILE *f) {
    List *current = list;
    while(current != NULL) {
        if(current->value != NULL) {
            fprintf(f, "%s ", current->value);
        }
        current = current->next;
    }
    fprintf(f, "\n");
}

void
keys_fprintf(BucketCell *cell, FILE *f) {
    BucketCell *current = cell;
    while(current != NULL) {
        // printf("Key %s: ", current->key);
        fprintf(f, "%s: ", current->key);
        list_fprintf(current->values, f);
        // printf("\n");
        current = current->next;
    }
}

void
map_fprintf(UnorderedMap *map, FILE *f) {
    for(int i = 0; i < (1 << 16); i++) {
        if(map->buckets[i] != NULL) {
            // printf("Hash %d:\n", i);
            keys_fprintf(map->buckets[i], f);
        }
    }
}

void
list_push_back(List *list, char *value) {
    List *current = list;
    while(current->next != NULL) {
        current = current->next;
    }
    current->next = (List *)calloc(1, sizeof(List));
    current->value = value;
}

void
map_insert(UnorderedMap *map, char *key, char *value) {
    unsigned short int ind = map->hash(key);
    BucketCell *cell = map->buckets[ind];
    if(cell == NULL) { // if no cells for current key 
        map->buckets[ind] = (BucketCell *)calloc(1, sizeof(BucketCell));
        map->buckets[ind]->key = key;
        map->buckets[ind]->values = (List *)calloc(1, sizeof(List));
        map->buckets[ind]->values->value = value;
        return;
    }
    BucketCell *current = cell;
    while(strcmp(current->key, key) != 0) {
        if(current->next == NULL) {
            break;
        }
        current = current->next;
    }
    if(current->next == NULL &&
       strcmp(current->key, key) != 0) { // no cell for such key, so create one 
        current->next = (BucketCell *)calloc(1, sizeof(BucketCell));
        current = current->next;
        current->key = key;
        current->values = (List *)calloc(1, sizeof(List));
        current->values->value = value;
        return;
    }

    List *currentValue = current->values;
    while(strcmp(currentValue->value, value) != 0) {
        if(currentValue->next == NULL) {
            break;
        }
        currentValue = currentValue->next;
    }
    if(currentValue->next == NULL && strcmp(currentValue->value, value) != 0) {
        currentValue->next = (List *)calloc(1, sizeof(List));
        currentValue = currentValue->next;
        currentValue->value = value;
        return;
    }
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
