#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "vector.h"

typedef int T;


typedef struct TreeNode {
    T data;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
} TreeNode;

typedef struct Tree {
    TreeNode *root;
} Tree;

// typedef struct Heap {
//     TreeNode *root;
//     size_t size;
// } Heap;

typedef Tree Heap;

typedef Tree BSTree;

extern Heap *heap_create(T data);
extern void heap_printf(Heap *heap);
extern void heap_insert(Heap *heap, T data);
extern T heap_erase_root(Heap *heap);
extern void bst_insert(BSTree *bst, T data);
extern void bst_erase(BSTree *bst, T data);
extern int bst_find(BSTree *bst, T data);
extern Vector *bst_to_vector(BSTree *bst);

#endif