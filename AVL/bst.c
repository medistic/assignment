#include "bst.h"
#include <stdlib.h>

static BSTNode *create_node(int value)
{
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (node == NULL) return NULL;
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void bst_init(BST *tree)
{
    if (tree == NULL) return;
    tree->root = NULL;
    tree->size = 0;
    tree->construction_comparisons = 0;
}

int bst_insert(BST *tree, int value)
{
    BSTNode *current, *parent = NULL, *new_node;

    if (tree == NULL) return 0;

    if (tree->root == NULL) {
        tree->root = create_node(value);
        if (tree->root == NULL) return 0;
        tree->size = 1;
        return 1;
    }

    current = tree->root;
    while (current != NULL) {
        tree->construction_comparisons++;
        parent = current;

        if (value == current->data) return 0;
        if (value < current->data) current = current->left;
        else current = current->right;
    }

    new_node = create_node(value);
    if (new_node == NULL) return 0;

    if (value < parent->data) parent->left = new_node;
    else parent->right = new_node;

    tree->size++;
    return 1;
}

int bst_search(const BST *tree, int key, int *comparisons)
{
    BSTNode *current;

    if (comparisons != NULL) *comparisons = 0;
    if (tree == NULL) return 0;

    current = tree->root;
    while (current != NULL) {
        if (comparisons != NULL) (*comparisons)++;

        if (key == current->data) return 1;
        if (key < current->data) current = current->left;
        else current = current->right;
    }
    return 0;
}

static int height_node(const BSTNode *node)
{
    int lh, rh;
    if (node == NULL) return 0;
    lh = height_node(node->left);
    rh = height_node(node->right);
    return (lh > rh ? lh : rh) + 1;
}

int bst_height(const BST *tree)
{
    if (tree == NULL) return 0;
    return height_node(tree->root);
}

static void destroy_node(BSTNode *node)
{
    if (node == NULL) return;
    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
}

void bst_destroy(BST *tree)
{
    if (tree == NULL) return;
    destroy_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
    tree->construction_comparisons = 0;
}
