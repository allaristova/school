#include "../headers/hlist.h"

#include <stdlib.h>
#include <stdio.h>


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
