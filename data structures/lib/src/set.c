#include "../headers/set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
hl_printf(List *list) {
    List *current = list;
    while (current != NULL) {
        printf("%s ", current->data);
        current = current->next;
    }
    //printf("\n");
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
set_printf(UnorderedSet *set) {
    for (int i = 0; i < (1 << 16); i++) {
        if (set->arr[i] != NULL) {
            //printf("Hash %d:\n", i);
            hl_printf(set->arr[i]);
        }
    }
    printf("\n");
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
hash_function(char *str) {
    return MurmurHash2(str, strlen(str));
}

void 
set_insert(UnorderedSet *set, char *str){
    unsigned short int ind = set->hash(str);
    List *cell = set->arr[ind];
    if(cell == NULL) {
        set->arr[ind] = hl_push_back(set->arr[ind], str);
        return;
    }
    List *current = cell;
    while(strcmp(current->data, str) != 0) {
        if(current->next == NULL)
            break;
        current = current->next;
    }
    if(current->next == NULL && strcmp(current->data, str) != 0) {
        current->next = (List*)calloc(1, sizeof(List));
        current = current->next;
        current->data = str;
        return;
    }
}


void 
set_erase(UnorderedSet *set, char *str){
    unsigned int ind = set->hash(str);
    List *list = set->arr[ind];
    if (list == NULL) {
        fprintf(stderr, "h_erase: this hash is NULL\n");
        fflush(stderr);
        exit(1); 
    } else {
        List *prev = NULL;
        List *current = list;
        while (current != NULL && strcmp(current->data, str) != 0) {
            prev = current;
            current = current->next;
        }
        if (current == NULL) {
            fprintf(stderr, "h_erase: there is no such element\n");
            fflush(stderr);
            exit(1); //элемент не найден
        }
        if (prev == NULL) {
            set->arr[ind] = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }
}


UnorderedSet *
set_create(){
    UnorderedSet *set = (UnorderedSet*)calloc(1, sizeof(UnorderedSet));
    set->hash = &hash_function;
    return set;
}

int
set_find(UnorderedSet *set, char *str) {
    unsigned int ind = set->hash(str);
    List *list = set->arr[ind];
    if (list == NULL) {
        return 0;
        // fprintf(stderr, "h_find: this hash is NULL\n");
        // fflush(stderr);
        // exit(1); 
    }
    List *current = list;
    while (current != NULL) {
        if (strcmp(current->data, str) == 0) {
            return 1; // строка найдена
        }
        current = current->next;
    }
    return 0; // строка не найдена
}

UnorderedSet *
set_intersection(UnorderedSet *set1, UnorderedSet *set2) {
    UnorderedSet *result_set = set_create();
    for (int i = 0; i < (1 << 16); i++) {
        List *list1 = set1->arr[i];
        List *list2 = set2->arr[i];
        
        List *current1 = list1;
        while (current1 != NULL) {
            if (set_find(set2, current1->data)) {
                set_insert(result_set, current1->data);
            }
            current1 = current1->next;
        }
    }
    
    return result_set;
}

void 
set_destroy(UnorderedSet *set){
    for(int i = 0; i < (1 << 16); ++i){
        List *current = set->arr[i];
        while(current != NULL){
            List *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(set);
    free(set);
}

