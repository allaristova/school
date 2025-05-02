#include <stdlib.h>
#include <stdio.h>

#include "../../lib/headers/hlist.h"


int
main(void) {
    List *list = (List*)calloc(1, sizeof(List));
    char *str = "ggg";
    char *str2 = "ggg2";
    list = hl_push_back(list, str);
    list = hl_push_back(list, str2);
    hl_printf(list);
    return 0;
}

