#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    int data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct {
    BSTNode *root;
    int size;
    long long construction_comparisons;
} BST;

void bst_init(BST *tree);
int bst_insert(BST *tree, int value);
int bst_search(const BST *tree, int key, int *comparisons);
int bst_height(const BST *tree);
void bst_destroy(BST *tree);

#endif
