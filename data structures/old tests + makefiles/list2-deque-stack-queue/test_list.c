#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../lib/headers/list.h"
#include "../../lib/headers/deque.h"
#include "../../lib/headers/stack.h"
#include "../../lib/headers/queue.h"

int
main(void) {


    L2 *list = (L2*)calloc(1, sizeof(L2));
    int n;
    int x;
    printf("%s\n", "enter the length of the list:");
    scanf("%d", &n);
    printf("%s\n", "enter the list items:");
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x);
        l2_push_back(list, x);
    }
    int flag = 1;
    while (flag != 0) {
        printf("%s\n", "enter <operation> ('push_back', 'insert', 'erase', 'update' or 'get') or 'exit'");
        char str;
        scanf("%s", &str);
        int operation;
        if (strcmp(&str, "push_back") == 0) {
            operation = 1;
        } else if (strcmp(&str, "insert") == 0) {
            operation = 2;
        } else if (strcmp(&str, "erase") == 0) {
            operation = 3;
        } else if (strcmp(&str, "update") == 0) {
            operation = 4;
        } else if (strcmp(&str, "get") == 0) {
            operation = 5;
        } else if (strcmp(&str, "exit") == 0) {
            operation = 0;
        } else {
            printf("%s\n", "there is no operation");
        }
        int elem, pos;
        switch (operation) {
            case 0:
                flag = 0;
                break;
            case 1:
                printf("%s\n", "enter the element:");
                scanf("%d", &elem);
                dq_push_back(list, elem);
                l2_printf(list);
                break;
            case 2:
                printf("%s\n", "enter the element and the index (<elem> <index>):");
                scanf("%d %d", &elem, &pos);
                l2_insert(list, elem, pos);
                l2_printf(list);
                break;
            case 3:
                printf("%s\n", "enter the index:");
                scanf("%d", &pos);
                l2_erase(list, pos);
                l2_printf(list);
                break;
            case 4:
                printf("%s\n", "enter the element and the index (<elem> <index>):");
                scanf("%d %d", &elem, &pos);
                l2_update(list, elem, pos);
                l2_printf(list);
                break;
            case 5:
                printf("%s\n", "enter the index:");
                scanf("%d", &pos);
                printf("%d\n", l2_get(list, pos));
                break;
            default:
                break;
        }
    }
    free(list);
    return 0;
}

