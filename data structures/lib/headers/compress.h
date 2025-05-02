#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>

typedef enum CompressType {
    MOCK_COMPRESS,
    RLE_COMPRESS,
    SIMPLE9_COMPRESS,
    FIBONACCI_COMPRESS,
    // COMPRESS_TYPE_2,
    // COMPRESS_TYPE_3, ...
} CompressType;

typedef struct CompressedData {
    CompressType mode;
    int n;
    void ***data;
} CompressedData;

extern CompressedData compress(CompressType mode, int n, void *data);

extern CompressedData mock_compress(int n, void *data);
extern CompressedData rle_compress(int n, void *data);
extern CompressedData simple9_compress(int n, void *data);
extern CompressedData fibonacci_compress(int n, void *data);

extern CompressedData rle_decompress(int n, void ***data);
extern CompressedData simple9_decompress(int n, void *data);
extern CompressedData fibonacci_decompress(int n, void *data);



#endif