#include "../headers/tree.h"

#include <stdio.h>
#include <stdlib.h>

Heap *
heap_create(T data) {
    TreeNode *root = (TreeNode*)calloc(1, sizeof(TreeNode));
    root->data = data;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;

    Heap *heap = (Heap*)calloc(1, sizeof(Heap));
    heap->root = root;

    return heap;
}


void 
TreeNode_printf(TreeNode* node) {
    if (node == NULL) {
        //printf("\n");
        return;
    }
    TreeNode_printf(node->left);
    printf("%d ", node->data);
    TreeNode_printf(node->right);
    return;
}

void
heap_printf(Heap *heap) {
    if (heap->root == NULL) {
        fprintf(stderr, "heap_printf: the heap is NULL\n");
        fflush(stderr);
        exit(1);
    }

    TreeNode_printf(heap->root);
    printf("\n"); 
    //printf("ok\n");
    return;
}

void 
swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}


void 
heap_up(Heap *heap, TreeNode *new_node) {
    while (new_node->data > new_node->parent->data && new_node->parent != NULL) {
        swap(&(new_node->data), &(new_node->parent->data));
        new_node = new_node->parent;
    }
}

void 
heap_insert(Heap *heap, T data) {
    if (heap->root == NULL) {
        heap = heap_create(data);
        return;
    }

    TreeNode *new_node = (TreeNode*)calloc(1, sizeof(TreeNode));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    TreeNode *current_node = heap->root;

    while (current_node->left != NULL && current_node->right != NULL) {
        //printf("%d\n", current_node->data);
        if (current_node->left != NULL && current_node->right != NULL) {
            if (current_node->left->data < current_node->right->data) {
                current_node = current_node->left;
            } else {
                current_node = current_node->right;
            }
        } else if (current_node->left != NULL) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }


    if (current_node->left == NULL) {
        current_node->left = new_node;
    } else {
        current_node->right = new_node;
    }
    new_node->parent = current_node;

    heap_up(heap, new_node);

}



T 
heap_erase_root(Heap *heap) {
if (heap->root == NULL) { //пустая куча
        fprintf(stderr, "heap_erase_root: the heap is NULL\n");
        fflush(stderr);
        exit(1);
    }

    T root_data = heap->root->data;
    TreeNode *oldRoot = heap->root;

    if (heap->root->left == NULL && heap->root->right == NULL) { //корень без детей
        heap->root = NULL;
        free(oldRoot);
        return root_data;
    } else if (heap->root->left == NULL) { //корень без левого ребенка
        heap->root = heap->root->right;
        heap->root->parent = NULL;
        free(oldRoot);
        return root_data;
    } else if (heap->root->right == NULL) { //корень без правого ребенка
        heap->root = heap->root->left;
        heap->root->parent = NULL;
        free(oldRoot);
        return root_data;
    } else { // есть оба ребенка 
        TreeNode *minNode = heap->root->right;
        while (minNode->left != NULL) {
            minNode = minNode->left;
        }

        if (minNode->parent->left == minNode) {
            minNode->parent->left = minNode->right;
        } else {
            minNode->parent->right = minNode->right;
        }

        if (minNode->right != NULL) {
            minNode->right->parent = minNode->parent;
        }

        minNode->left = heap->root->left;
        heap->root->left->parent = minNode;
        minNode->right = heap->root->right;
        heap->root->right->parent = minNode;

        heap->root = minNode;
        minNode->parent = NULL;

        free(oldRoot);
        return root_data;
    }

    return root_data;
}

TreeNode 
*bst_create_node(T data) {
    TreeNode *new_node = (TreeNode*)calloc(1, sizeof(TreeNode));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    return new_node;
}

void 
bst_insert(BSTree *bst, T data) {
    TreeNode *new_node = bst_create_node(data);

    if (bst->root == NULL) {
        bst->root = new_node;
        return;
    }

    TreeNode *current_node = bst->root;
    while (current_node != NULL) {
        new_node->parent = current_node;
        if (data <= current_node->data) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }

    if (data <= new_node->parent->data) {
        new_node->parent->left = new_node;
    } else {
        new_node->parent->right = new_node;
    }
}

void 
BstNode_erase(TreeNode **current_node, T data) {
    TreeNode *delete_node = *current_node;
    if (delete_node == NULL) {
        return;
    }

    if (data < delete_node->data) {
        BstNode_erase(&(delete_node->left), data);
    } else if (data > delete_node->data) {
        BstNode_erase(&(delete_node->right), data);
    } else {
        if (delete_node->left == NULL) {
            *current_node = delete_node->right;
            free(delete_node);
            return;
        } else if (delete_node->right == NULL) {
            *current_node = delete_node->left;
            free(delete_node);
            return;
        } else {
            TreeNode *min_right = delete_node->right;
            while (min_right->left != NULL) {
                min_right = min_right->left;
            }
            delete_node->data = min_right->data;
            BstNode_erase(&(delete_node->right), min_right->data);
        }
    }
}

void 
bst_erase(BSTree *bst, T data) {
    if (bst->root != NULL) {
        BstNode_erase(&(bst->root), data);
    }
}

int
bst_find(BSTree *bst, T data) {
    TreeNode *current_node = bst->root;
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

void 
TreeNode_to_vector(TreeNode* node, Vector *vector) {
    if (node == NULL) {
        //printf("\n");
        return;
    }
    TreeNode_to_vector(node->left, vector);
    v_push_back(vector, node->data);
    TreeNode_to_vector(node->right, vector);
    return;
}

enum { 
    SIZE = 1,
    CAPACITY = 100000
};

Vector *
bst_to_vector(BSTree *bst) {
    if (bst->root == NULL) {
        fprintf(stderr, "bst_to_vector: the heap is NULL\n");
        fflush(stderr);
        exit(1);
    }
    T *arr = calloc(1, sizeof(*arr));
    Vector *vector = calloc(1, sizeof(vector));
    vector->size = SIZE;
    vector->capacity = CAPACITY;
    vector->data = arr;
    
    TreeNode_to_vector(bst->root, vector);
    v_erase(vector, 0);
    //printf("ok\n");
    return vector;
}


