#include <stdio.h>
#include <stdlib.h>
#include "../../lib/headers/compress.h"

int
main(void) {
    FILE *input;
    input = fopen("./input.txt", "r");
    int n = 0;
    char symbol;
    while((symbol = getc(input)) != EOF) {
         n++;
    }

    char *arr;
    arr = calloc(n, sizeof(char*));
    fseek(input, 0L, SEEK_SET);
    for(int i = 0; i < n; ++i) {
        arr[i] = getc(input);
    }
    fclose(input);

    CompressedData compressed;
    compressed = compress(RLE_COMPRESS, n, arr);
    // MOCK_COMPRESS,
    // RLE_COMPRESS,
    // SIMPLE9_COMPRESS,
    // FIBONACCI_COMPRESS,

    FILE *output;
    output = fopen("./output.txt", "w");
    for (int i = 0; i < compressed.n; i++) {
       for (int j = 0; j < 2; j++) {
            fprintf(output, "%d ", compressed.data[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);

    return 0;    
}
