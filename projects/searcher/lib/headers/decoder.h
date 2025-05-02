#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    unsigned char symb;
    unsigned long int bits;
    char nbits;
    struct Node *c0;
    struct Node *c1;
} Node;

void del_tree(Node *n) {
    if (n->c0) {
        del_tree(n->c0);
    }
    if (n->c1) {
        del_tree(n->c1);
    }
    free(n);
}

void
decode(char *input_path, char *output_path) {
    Node *root;
    long int f_size;
    int n_el;
    unsigned long int bits = 0;
    Node *new_n, *cur;
    unsigned char a;
    int j;

    root = (Node *)calloc(1, sizeof(Node));
    root->symb = 0;
    root->c1 = root->c0 = NULL;

    FILE *inp = fopen(input_path, "rb");
    fread(&f_size, sizeof(long int), 1, inp);
    fread(&n_el, sizeof(int), 1, inp);
    for (int i = 0; i < n_el; i++) {
        new_n = (Node *)calloc(1, sizeof(Node));
        fread(&new_n->symb, sizeof(char), 1, inp);
        fread(&new_n->bits, sizeof(unsigned long int), 1, inp);
        fread(&new_n->nbits, sizeof(char), 1, inp);
        new_n->c0 = new_n->c1 = NULL;
        j = 1 << (new_n->nbits - 1);
        cur = root;
        while (j > 1) {
            if (new_n->bits & j) {  // c1
                if (cur->c1)
                    cur = cur->c1;
                else {
                    cur->c1 = (Node *)calloc(1, sizeof(Node));
                    cur = cur->c1;
                    cur->symb = 0;
                    cur->c1 = cur->c0 = NULL;
                }
            } else {  // c0
                if (cur->c0)
                    cur = cur->c0;
                else {
                    cur->c0 = (Node *)calloc(1, sizeof(Node));
                    cur = cur->c0;
                    cur->symb = 0;
                    cur->c1 = cur->c0 = NULL;
                }
            }
            j >>= 1;
        }
        // last bit
        if (new_n->bits & 1)  // c1
            cur->c1 = new_n;
        else  // c0
            cur->c0 = new_n;
    }

    // out
    FILE *out = fopen(output_path, "w");
    fread(&a, sizeof(char), 1, inp);
    bits |= a;
    bits <<= 8;
    fread(&a, sizeof(char), 1, inp);
    bits |= a;
    bits <<= 8;
    fread(&a, sizeof(char), 1, inp);
    bits |= a;
    bits <<= 8;
    fread(&a, sizeof(char), 1, inp);
    bits |= a;
    j = 0;
    cur = root;

    do {
        if (bits & 0x80000000)
            cur = cur->c1;
        else
            cur = cur->c0;  // подходит
        bits <<= 1;
        j++;
        if (8 == j) {
            int i = fread(&a, sizeof(char), 1, inp);
            bits |= a;
            j = 0;
        }
        if (!cur->c1 && !cur->c0) {
            putc(cur->symb, out);
            f_size--;
            cur = root;
        }
    } while (f_size);

    fclose(out);
    fclose(inp);
    del_tree(root);
    return;
}