#ifndef HASH_H
#define HASH_H

#include <stdio.h>


typedef struct List {
    char *data;
    struct List *next;
} List;

typedef struct HashTable {
    unsigned int (*hash)(char *str);
    List *arr[1 << 16];
} HashTable;

extern HashTable *h_create();
extern void h_printf(HashTable *hash_table);
extern unsigned int hash_function(char *str);
extern void h_insert(HashTable *hash_table, char *str);
extern void h_erase(HashTable *hash_table, char *str);
extern int h_find(HashTable *hash_table, char *str);
//extern char *h_get(HashTable *hash_table, char *str);

#endif

