#include "../lib/headers/decoder.h"
#include "../lib/headers/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

UnorderedMap *
unpackIndex(char *index_path) {
    UnorderedMap *map = map_create();
    decode(index_path, "./index.txt");
    FILE *f = fopen("./index.txt", "r");
    int line_length = 0;
    int len = 0;
    char *line = NULL;
    while((line_length = getline(&line, &len, f)) != -1) {
        char *_line = (char *)calloc(sizeof(line), sizeof(char *));
        strcpy(_line, line);
        char *token = strtok(_line, ":");
        // free(_line);
        char *file_name = strtok(line, " ");
        while(file_name != NULL) {
            file_name = strtok(NULL, " ");
            if(file_name == NULL)
                break;

            char *_file_name = (char *)calloc(sizeof(file_name), sizeof(char *));
            strcpy(_file_name, file_name);
            _file_name[strcspn(file_name, "\n")] = 0; // remove newline
            if(token == NULL || *_file_name == 0)
                continue;
            //printf("token: |%s|  ---  filename: |%s|\n", token, _file_name);
            map_insert(map, token, _file_name);
            // free(_file_name);
        }
    }
    return map;
}

int
main(int argc, char **argv)
{
    if(argc == 1) {
        printf("%s\n%s\n", "Error! No archive provided", "Usage: ./search archive.compress");
        return 1;
    }

    char *archivePath = argv[1];
    int n;
    printf("%s", "Enter number of queries: ");
    scanf("%d%*c", &n);
    UnorderedMap *indexes = unpackIndex(archivePath);
    printf("done\n");
    for(int q = 0; q < n; q++) {
        printf("query #%d: ", q + 1);
        char *query = NULL;
        size_t len = 0;
        getline(&query, &len, stdin);
        char *word = strtok(query, " ");
        int word_count = 0;
        CounterList *counter = counter_create();
        while(word != NULL) {
            word[strcspn(word, "\r\n")] = 0;
            List *words_by_index = map_get(indexes, word);
            word_count++;
            if(words_by_index == NULL)
                break;
            counter_insert_list(counter, words_by_index);
            word = strtok(NULL, " ");
        }
        List *answer = get_list_with_count(counter, word_count);
        list_printf(answer);
        printf("\ndone query #%d\n", q + 1);
    }
}
