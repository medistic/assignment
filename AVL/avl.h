#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    int data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct {
    AVLNode *root;
    int size;
    long long construction_comparisons;
} AVLTree;

void avl_init(AVLTree *tree);
int avl_insert(AVLTree *tree, int value);
int avl_search(const AVLTree *tree, int key, int *comparisons);
int avl_height(const AVLTree *tree);
void avl_destroy(AVLTree *tree);

#endif
