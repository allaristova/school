#include <stdio.h>

#include "../../lib/headers/set.h"

int
main(void) {
    UnorderedSet *set = set_create();
    UnorderedSet *set2 = set_create();
    
    // printf("kkk\n");
    
    // char *str = "pupupu";
    set_insert(set, "ttt");
    set_insert(set, "gggg");
    set_insert(set, "ttt");
    set_insert(set, "jvwvck");
    set_insert(set2, "ttt");
    set_insert(set2, "jdsck");
    set_insert(set2, "jvwvck");
    UnorderedSet *set3 = set_create();
    set3 = set_intersection(set, set2);
    set_printf(set3);
    set_printf(set);
    set_printf(set2);
    // h_insert(hash_table, "jjj");
    // h_insert(hash_table,"vmlavg");
    // h_insert(hash_table, "pupupu");
    // h_printf(hash_table);
    // printf("\n------\n");
    // h_erase(hash_table, "jjj");
    // //h_erase(hash_table, "kllm");
    // h_printf(hash_table); 
    // printf("\n------\n");
    // printf("%d\n", h_find(hash_table, "vmlavg")); 
    // printf("%d\n", h_find(hash_table, "jjj")); 
    // //free(hash_table);
    return 0;
}