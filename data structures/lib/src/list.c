#include "../headers/list.h"

#include <stdlib.h>
#include <stdio.h>


void
l_printf(List *list) {
    printf("%d ", list->data);
    if (list->next != NULL){
        List *current = list->next;

        while (current != NULL) {
            printf("%d ", current->data);
            current = current->next;
        }
        printf("\n");
    }
}

List *
l_push_back(List *list, T elem) {
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

List *
l_insert(List *list, T elem, int pos) {
    // if (list == NULL){
    //     printf("%s\n", "the list is NULL");
    //     exit(1);
    // }
    List *new_elem = (List*)calloc(1, sizeof(List));
    new_elem->data = elem;

    if (pos == 0) {
        new_elem->next = list;
        return new_elem;
    }

    List *new_list = list;
    List *current = list;
    int count = 0;

    while (current != NULL && count < pos - 1) {
        current = current->next;
        count++;
    }
    if (current == NULL) {
        printf("%s\n", "there is no element in the list at the specified index");
        return NULL;
        //exit(1);
    }
    new_elem->next = current->next;
    current->next = new_elem;
    return new_list;
}

List *
l_erase(List *list, int pos) {
    if (list == NULL){
        printf("%s\n", "the list is NULL");
        exit(1);
    }
    if (pos == 0) {
        List *new_list = list->next;
        free(list);
        return new_list;
    }
    List *current = list;
    int count = 0;
    while (current != NULL && count < pos - 1) {
        current = current->next;
        count++;
    }
    if (current->next == NULL) {
        printf("%s\n", "there is no element in the list at the specified index");
        exit(1);
    }
    List *elem_to_delete = current->next;
    current->next = elem_to_delete->next;
    free(elem_to_delete);
    return list;
}

List *
l_update(List *list, T elem, int pos) {
    if (list == NULL){
        printf("%s\n", "the list is NULL");
        exit(1);
    }

    if (pos == 0) {
        list->data = elem;
        return list;
    }
    List *new_list = list;

    int count = 0;
     for (int i = 0; i < pos && list->next != NULL; ++i) {
        list = list->next;
        count++;
    }

    if (list->next == NULL) {
        printf("%s\n", "there is no element in the list at the specified index");
        exit(1);
    }

    list->data = elem;
    return new_list;
}


T
l_get(List *list, int pos) {
    if (list == NULL){
        printf("%s\n", "the list is NULL");
        exit(1);
    }
    int count = 0;
    while((count != pos) && (list->next != NULL)) {
        list = list->next;
        count++;
    }

    if (list->next == NULL) {
        printf("%s\n", "there is no element in the list at the specified index");
        exit(1);
    }

    return list->data;
}

List *
l_find(List *list, T elem) {
    while((list->data != elem) && (list->next != NULL)) {
        list = list->next;
    }
    if (list->next == NULL) {
        printf("%s\n", "there is no element in the list at the specified index");
        exit(1);
    }
    return list;
}

List *
l_remove_duplicates(List *list) {
    List *elem = (List*)calloc(1, sizeof(List));
    elem = list;
    List *new_list = (List*)calloc(1, sizeof(List));

    while (elem != NULL) {
        int flag = 0;
        List *temp = new_list;
        while (temp != NULL) {
            if (temp->data == elem->data) {
                flag = 1;
            }
            temp = temp->next;
        }
        if (flag == 0) {
            l_push_back(new_list, elem->data);
        }
        elem = elem->next;
    }
    return new_list->next;
}



void
l2_printf(L2 *list) {
    if (list->head == NULL) {
        printf("the list is empty\n");
        return;
    }
    L2Node *current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void
l2_push_back(L2 *list, T elem) {
    if (list == NULL) {
        fprintf(stderr, "l2_push_back: list is NULL\n");
        fflush(stderr);
        exit(1);
    }
    if (list->size == 0) {
        list->head = calloc(1, sizeof(*list->head));
        list->head->data = elem;
        list->head->next = NULL;
        list->head->prev = NULL;
        list->tail = list->head;
        list->size++;
    } else {
        list->tail->next = calloc(1, sizeof(*list->head));
        list->tail->next->data = elem;
        list->tail->next->next = NULL;
        list->tail->next->prev = list->tail;
        list->tail = list->tail->next;
        list->size++;
    }
}

void
l2_push_front(L2 *list, T elem) {
    if (list == NULL) {
        fprintf(stderr, "l2_push_front: list is NULL\n");
        fflush(stderr);
        exit(1);
    }
    if (list->size == 0) {
        list->head = calloc(1, sizeof(*list->head));
        list->head->data = elem;
        list->head->next = NULL;
        list->head->prev = NULL;
        list->tail = list->head;
        list->size++;
    } else {
        list->head->prev = calloc(1, sizeof(*list->head));
        list->head->prev->data = elem;
        list->head->prev->prev = NULL;
        list->head->prev->next = list->head;
        list->head = list->head->prev;
        list->size++;
    }
}

void
l2_insert(L2 *list, T elem, int pos) {
    if (list == NULL) {
        fprintf(stderr, "l2_insert: list is NULL\n");
        fflush(stderr);
        exit(1);
    }

    if (pos < 0 || pos > list->size) {
        fprintf(stderr, "l2_insert: invalid position\n");
        fflush(stderr);
        exit(1);
    }

    if (pos == 0) {
        l2_push_front(list, elem);
        return;
    }

    if (pos == list->size) {
        l2_push_back(list, elem);
        return;
    }

    L2Node *new_node = calloc(1, sizeof(*new_node));
    new_node->data = elem;
    L2Node *current = list->head;
    for (int i = 0; i < pos - 1; ++i) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next->prev = new_node;
    new_node->prev = current;
    current->next = new_node;
    list->size++;
}

void
l2_erase(L2 *list, int pos) {
    if (list == NULL) {
        fprintf(stderr, "l2_push_back: list is NULL\n");
        fflush(stderr);
        exit(1);
    }

    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "l2_insert: invalid position\n");
        fflush(stderr);
        exit(1);
    }

    if (pos == 0) {
        L2Node *elem_to_delete = list->head;
        list->head = list->head->next;
        if (list->head != NULL) {
            list->head->prev = NULL;
        } else {
            list->tail = NULL;
        }
        free(elem_to_delete);
        list->size--;
        return;
    }

    if (pos == list->size - 1) {
        L2Node *elem_to_delete = list->tail;
        list->tail = list->tail->prev;
        if (list->tail != NULL) {
            list->tail->next = NULL;
        } else {
            list->head = NULL;
        }
        free(elem_to_delete);
        list->size--;
        return;
    }

    if (pos < list->size / 2) {
        L2Node *current = list->head;
        for (int i = 0; i < pos - 1; ++i) {
            current = current->next;
        }
        L2Node *elem_to_delete = current->next;
        current->next = current->next->next;
        if (current->next != NULL) {
            current->next->prev = current;
        } else {
            list->tail = NULL;
        }
        free(elem_to_delete);
    } else {
        L2Node *current = list->tail;
        for (int i = list->size - 1; i > pos + 1; --i) {
            current = current->prev;
        }
        L2Node *elem_to_delete = current->prev;
        current->prev = current->prev->prev;
        if (current->prev != NULL) {
            current->prev->next = current;
        } else {
            list->head = current;
        }
        free(elem_to_delete);
    }
    list->size--;
}


void
l2_update(L2 *list, T elem, int pos) {
    if (list == NULL) {
        fprintf(stderr, "l2_update: list is NULL\n");
        fflush(stderr);
        exit(1);
    }

    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "l2_update: invalid position\n");
        fflush(stderr);
        exit(1);
    }
    if (pos == 0) {
        list->head->data = elem;
        return;
    }

    if (pos < list->size / 2) {
        L2Node *current = list->head;
        for (int i = 0; i < pos; ++i) {
            current = current->next;
        }
        current->data = elem;
    } else {
        L2Node *current = list->tail;
        for (int i = list->size - 1; i > pos; --i) {
            current = current->prev;
        }
        current->data = elem;
    }

}

T
l2_get(L2 *list, int pos){
    if (list == NULL) {
        fprintf(stderr, "l2_update: list is NULL\n");
        fflush(stderr);
        exit(1);
    }

    if (pos < 0 || pos >= list->size) {
        fprintf(stderr, "l2_update: invalid position\n");
        fflush(stderr);
        exit(1);
    }
    if (pos == 0) {
        return list->head->data;
    }
    if (pos == list->size - 1) {
        return list->tail->data;
    }

    if (pos < list->size / 2) {
        L2Node *current = list->head;
        for (int i = 0; i < pos; ++i) {
            current = current->next;
        }
        return current->data;
    } else {
        L2Node *current = list->tail;
        for (int i = list->size - 1; i > pos; --i) {
            current = current->prev;
        }
        return current->data;
    }
}

L2Node
*l2_find(L2 *list, T elem){
    if (list == NULL) {
        fprintf(stderr, "l2_find: list is NULL\n");
        fflush(stderr);
        exit(1);
    }
    L2Node *current = list->head;
    while (current != NULL) {
        if (current->data == elem) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}