#ifndef AVL_H
#define AVL_H

#include <stdio.h>

typedef int T;

typedef struct AVLNode {
    T data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
    struct AVLNode *parent;
} AVLNode;

typedef struct AVLTree {
    AVLNode *root;
} AVLTree;


extern int avl_find(AVLTree *avl, T data);
extern void avl_insert(AVLTree *avl, T data);
extern void avl_erase(AVLTree *avl, T data);

#endif