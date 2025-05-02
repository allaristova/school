#include "../headers/hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <keying.h>


void
hl_printf(List *list) {
    List *current = list;
    while (current != NULL) {
        printf("%s ", current->data);
        current = current->next;
    }
    printf("\n");
}

List *
hl_push_back(List *list, char *elem) {
    List *last_elem = (List*)calloc(1, sizeof(List));
    last_elem->data = elem;
    last_elem->next = NULL;
    if (list == NULL){
        return last_elem;
    }
    List *current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = last_elem;
    return list;
}

void
h_printf(HashTable *hash_table) {
    for (int i = 0; i < (1 << 16); i++) {
        if (hash_table->arr[i] != NULL) {
            printf("Hash %d:\n", i);
            hl_printf(hash_table->arr[i]);
        }
    }
}

unsigned short int
MurmurHash2(char *key, unsigned int len) {
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;
    unsigned int k = 0;

    while (len >= 4) {
        k  = data[0];
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

    switch (len) {
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
hash_function(char *key) {
    return MurmurHash2(key, keylen(key));
}

HashTable *
h_create() {
    HashTable *hash_table = (HashTable*)calloc(1, sizeof(HashTable));
    hash_table->hash = &hash_function;
    return hash_table;
}



void
h_insert(HashTable *hash_table, char *key, char * value) {
    unsigned short int ind = hash_table->hash(key);
    List *list = hash_table->arr[ind];
    if (list == NULL) {
        //hash_table->arr[ind] = (List*)calloc(1, sizeof(List));
        hash_table->arr[ind] = hl_push_back(hash_table->arr[ind], key);
    } else {
        List *current = list;
        while (current->next != NULL) {
            if (keycmp(current->data, key) == 0) {
                printf("h_insert: there is already such an element\n");
                return;
            }
            current = current->next;
        }
        current->next = (List*)calloc(1, sizeof(List));
        current->next->data = keydup(key);
    }
}

void
h_erase(HashTable *hash_table, char *key) {
    unsigned int ind = hash_table->hash(key);
    List *list = hash_table->arr[ind];
    if (list == NULL) {
        fprintf(stderr, "h_erase: this hash is NULL\n");
        fflush(stderr);
        exit(1); 
    } else {
        List *prev = NULL;
        List *current = list;
        while (current != NULL && keycmp(current->data, key) != 0) {
            prev = current;
            current = current->next;
        }
        if (current == NULL) {
            fprintf(stderr, "h_erase: there is no such element\n");
            fflush(stderr);
            exit(1); //элемент не найден
        }
        if (prev == NULL) {
            hash_table->arr[ind] = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }
}

int 
h_find(HashTable *hash_table, char *key) {
    unsigned int ind = hash_table->hash(key);
    List *list = hash_table->arr[ind];
    if (list == NULL) {
        return 0;
        // fprintf(stderr, "h_find: this hash is NULL\n");
        // fflush(stderr);
        // exit(1); 
    }
    List *current = list;
    while (current != NULL) {
        if (keycmp(current->data, key) == 0) {
            return 1; // строка найдена
        }
        current = current->next;
    }
    return 0; // строка не найдена
}


