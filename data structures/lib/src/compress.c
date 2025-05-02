#include "../headers/compress.h"

#include <stdio.h>
#include <stdlib.h>


CompressedData
mock_compress(int n, void *data) {
    CompressedData compressed;
    compressed.n = n;
    compressed.data = data;
    return compressed;
}

CompressedData
rle_compress(int n, void *data) {
    CompressedData compressed;
    //compressed = calloc(1, sizeof(*compressed));
    char *data_arr = (char*)data;
    char current = data_arr[0];
    int count = 1;

    char **compressed_data = calloc(n, sizeof(*compressed_data));
    char *compressed_data_0 = calloc(n * 2, sizeof(*compressed_data_0));

    for (int i = 0; i < n; ++i) {
        compressed_data[i] = &compressed_data_0[i * 2];
    }
    int pos = 0;

    for (int i = 1; i < n; ++i) {
        if (data_arr[i] == current) {
            count++;
        } else {
            compressed_data[pos][0] = current;
            compressed_data[pos][1] = count;
            count = 1;
            current = data_arr[i];
            pos++;
        }
    }
    compressed.n = pos;
    compressed.data = calloc(compressed.n, sizeof(char*));
    for(int i = 0 ; i < compressed.n; i++) {
        compressed.data[i] = calloc(2, sizeof(char));
    }
    //char **compressed_data_new = calloc(pos, sizeof(*compressed_data_new));

    for (int i = 0; i < pos; ++i) {
        compressed.data[i][0] = compressed_data[i][0];
         compressed.data[i][1] = compressed_data[i][1];
    }

    //compressed->data = (char*)compressed_data_new;
    for (int i = 0; i < compressed.n; i++) {
       for (int j = 0; j < 2; j++) {
            printf("%d ", compressed.data[i][j]);
            }
        printf("\n");
    }
    for (int i = 0; i < compressed.n; i++) {
       for (int j = 0; j < 2; j++) {
            printf("%d ", compressed_data[i][j]);
            }
        printf("\n");
    }
    free(compressed_data_0);
    free(compressed_data);
    return compressed;

}


CompressedData
fibonacci_compress(int n, void *data) {
    CompressedData compressed;
    compressed.data = (char*)calloc(n, sizeof(char*));

    int FIB_CONST = 45;
    int num = atoi(data), fib[FIB_CONST];
    fib[0] = 1;
    fib[1] = 2;
    for (int i = 2; i < FIB_CONST; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    int i = FIB_CONST - 1;
    while (fib[i] > num) {
        i--;
    }
    int j = 1;
    compressed.n = i;
    while (i >= 0) {
        if (num >= fib[i]) {
            compressed.data[j] = '1';
            num -= fib[i];
        } else {
            compressed.data[j] = '0';
        }
        j++;
        i--;
    }
    compressed.data[0] = '1';
    return compressed;
}


CompressedData 
simple9_compress(int n, void *data) {
    CompressedData compressed;
    char *data_arr = (char*)data;
    int pos = 0;

    compressed.data = (char**)calloc(n, sizeof(char*));
    for (int i = 0; i < n; i++) {
        compressed.data[i] = calloc(3, sizeof(char));  
    }

    int mode = 0; 
    for (int i = 0; i < n; i++) {
        int value = 0;
        int bits = 0;

        switch (mode) {
            case 0:  // 1
                value = data_arr[i] & 0x1;
                bits = 1;
                break;
            case 1:  // 2
                value = data_arr[i] & 0x3;
                bits = 2;
                break;
            case 2:  // 3
                value = data_arr[i] & 0x7;
                bits = 3;
                break;
            case 3:  // 4
                value = data_arr[i] & 0xF;
                bits = 4;
                break;
            case 4:  // 5
                value = data_arr[i] & 0x1F;
                bits = 5;
                break;
            case 5:  // 7
                value = data_arr[i] & 0x7F;
                bits = 7;
                break;
            case 6:  // 9
                value = data_arr[i] & 0x1FF;
                bits = 9;
                break;
            case 7:  // 14
                value = data_arr[i] & 0x3FFF;
                bits = 14;
                break;
            case 8:  // 28
                value = data_arr[i];  // все биты
                bits = 28;
                break;
        }
        //sprintf(compressed.data[pos], "%0*d", bits, value);
        if (i % 3 == 2 && mode < 8) {
            mode++;
        }

        pos++;
    }

    compressed.n = pos;
    return compressed;
} 

CompressedData
rle_decompress(int n, void ***data) {
    int num = 0;
    char *decompressed_data;
    char count;
    for (int i = 0; i < n; i++) {
        count = data[i][1];
        num += count;
    }
    decompressed_data = calloc(num, sizeof(char));
    int pos = 0;
    for (int i = 0; i < n; ++i) {
        char symbol = data[i][0];
        char count = data[i][1];
        for (int j = 0; j < count; ++j) {
            decompressed_data[pos++] = symbol;
        }
    }
    CompressedData decompressed;
    decompressed.n = num;
    decompressed.data = decompressed_data;
    return decompressed;
}


CompressedData
fibonacci_decompress(int n, void *data) {
    CompressedData decompressed;
    decompressed.data = (char*)calloc(n, sizeof(char*));
    int FIB_CONST = 45;
    int num = 0;
    char *fib;
    fib  = (char*)calloc(FIB_CONST, sizeof(*fib));
    fib[0] = 1;
    fib[1] = 2;
    for (int i = 2; i < FIB_CONST; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    int j = 1;
    for (int i = n - 1; i >= 0; i--) {
      /* if (data[j] == 1) {
            num += fib[i];
        }
        j++; */
    }
    decompressed.n = num;
    return decompressed;
}

CompressedData
compress(CompressType mode, int n, void *data) {

    CompressedData result;
    result.mode = mode;

    switch (mode) {
        case MOCK_COMPRESS:
            result = mock_compress(n, data);
            break;
        case RLE_COMPRESS:
            result = rle_compress(n, data);
            break;
        case SIMPLE9_COMPRESS:
            simple9_compress(n, data);
            break;
        case FIBONACCI_COMPRESS:
            fibonacci_compress(n, data);
            break;
        // case COMPRESS_TYPE_2: ...
    }

    return result;
}
