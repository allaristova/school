#include <stdio.h>

#include "../../lib/headers/map.h"

int
main(void) {
    UnorderedMap *map = map_create();
    
    printf("kkk\n");
    map_insert(map, "vasya", "strah");
    map_printf(map);
    
    // char *str = "pupupu";
    // h_insert(hash_table, str);
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