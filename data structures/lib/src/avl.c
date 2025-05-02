#include "../headers/avl.h"

#include <stdio.h>
#include <stdlib.h>


int
h(AVLNode *node) {
    if (node == NULL) {
        return 0;
    } else {
        return node->height;
    }
}


int
max(int a, int b) {
    return a > b ? a : b;
}


AVLNode *
avlNode_create(T data) {
    AVLNode *new_node = calloc(1, sizeof(*new_node));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->height = 1;
    return new_node;
}


AVLNode *
rotate_left(AVLNode *root) { //малый поворот налево, root - вершина этой части
    if (root == NULL) {
        fprintf(stderr, "rotate_left: the AVL is NULL\n");
        fflush(stderr);
        exit(1);
    }
    AVLNode *res = root->right;
    AVLNode *right_left_son = root->right->left;
    res->left = root;
    res->left->right = right_left_son;
    res->left->height = max(h(res->left->left), h(res->left->right) + 1);
    res->height = max(h(res->left), h(res->right)) + 1;
    return res;
}

AVLNode *
rotate_right(AVLNode *root) { //малый поворот направо, root - вершина этой части
    if (root == NULL) {
        fprintf(stderr, "rotate_right: the AVL is NULL\n");
        fflush(stderr);
        exit(1);
    }
    AVLNode *res = root->left;
    AVLNode *left_right_son = root->left->right;
    res->right = root;
    res->right->left = left_right_son;
    res->right->height = max(h(res->right->right), h(res->right->left) + 1);
    res->height = max(h(res->right), h(res->left)) + 1;
    return res;
}

AVLNode *
big_rotate_right(AVLNode *root) {
    root->left = rotate_left(root->left);
    return rotate_right(root);
}
AVLNode *
big_rotate_left(AVLNode *root) {
    root->right = rotate_right(root->right);
    return rotate_left(root);
}

int
avl_find(AVLTree *avl, T data) {
    AVLNode *current_node = avl->root;
    while (current_node != NULL) {
        if (data < current_node->data) {
            current_node = current_node->left;
        } else if (data > current_node->data) {
            current_node = current_node->right;
        } else if (data == current_node->data) {
            return 1;
        }
    }
    return 0;
}


int
balance(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    int h_left = h(node->left);
    int h_right = h(node->right);

    if (node->left != NULL) {
        h_left++;
    }

    if (node->right != NULL) {
        h_right++;
    }
    return h_left - h_right;
}

AVLNode *
avlNode_insert(AVLNode *node,  T data) {
    if (node == NULL) {
        node = avlNode_create(data);
        return node;
    }

    if (data < node->data) {
        AVLNode* left_child = avlNode_create(data);
        node->left = left_child;
        left_child->parent = node;
        if (balance(node) == 2) {
            if (data < node->left->data) {
                node = rotate_right(node);
            } else {
                node = big_rotate_right(node);
            }
        }
    } else if (data > node->data) {
        AVLNode* right_child = avlNode_create(data);
        node->right = right_child;
        right_child->parent = node;
        if (balance(node) == -2) {
            if (data > node->right->data) {
                node = rotate_left(node);
            } else {
                node = big_rotate_left(node);
            }
        }
    } 
    node->height = max(h(node->left), h(node->right)) + 1;
    return node;
}

void
avl_insert(AVLTree *avl, T data) {
    if (avl == NULL) {
        fprintf(stderr, "avl_insert: the AVL is NULL\n");
        fflush(stderr);
        exit(1);
    }
    avl->root = avlNode_insert(avl->root, data);
}

AVLNode *
avlNode_delete(AVLNode *node, T data) {
    if (node == NULL) {
        fprintf(stderr, "avl_delete: there is no such element in the AVL\n");
        fflush(stderr);
        exit(1);
    }
    if (data < node->data) {
        node->left = avlNode_delete(node->left, data);
        if (balance(node) == -2) {
            if (balance(node->right) <= 0) {
                node = rotate_left(node);
            } else {
                node = big_rotate_left(node);
            }
        }
    } else if (data > node->data) {
        node->right = avlNode_delete(node->right, data);
        
        if (balance(node) == 2) {
            if (balance(node->left) >= 0) {
                node = rotate_right(node);
            } else {
                node = big_rotate_right(node);
            }
        }
    } else { // элемент найден
        if (node->right == NULL) { //если нет правого сына - ставим на его место левого
            AVLNode *left_son = node->left;
            left_son->parent = node->parent;
            free(node);
            return left_son;
        } else { // если есть правый сын
    //присваиваем наименьшее значение (ищем самого левого потомка в правом поддереве)
            AVLNode *current = node->right;
            while (current->left != NULL) {
                current = current->left;
            }
            node->data = current->data;
            node->right = avlNode_delete(node->right, node->data);
            if (balance(node->left) >= 0) {
                node = rotate_right(node);
            } else {
                node = big_rotate_right(node);
            }

        }
    }
    node->height = max(h(node->left), h(node->right)) + 1;
    return node;
}

void
avl_delete(AVLTree *avl, T data) {
    if (avl == NULL) {
        fprintf(stderr, "avl_delete: the AVL is NULL\n");
        fflush(stderr);
        exit(1);
    }
    avl->root = avlNode_delete(avl->root, data);
}