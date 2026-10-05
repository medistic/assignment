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
    tree->build_comparisons = 0;
}

int bst_insert(BST *tree, int value)
{
    BSTNode *current;
    BSTNode *parent = NULL;
    BSTNode *new_node;

    if (tree == NULL) return 0;

    if (tree->root == NULL) {
        tree->root = create_node(value);
        return tree->root != NULL;
    }

    current = tree->root;

    while (current != NULL) {
        tree->build_comparisons++;
        parent = current;

        if (value < current->data)
            current = current->left;
        else if (value > current->data)
            current = current->right;
        else
            return 0;
    }

    new_node = create_node(value);
    if (new_node == NULL) return 0;

    if (value < parent->data)
        parent->left = new_node;
    else
        parent->right = new_node;

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

        if (key == current->data)
            return 1;
        else if (key < current->data)
            current = current->left;
        else
            current = current->right;
    }

    return 0;
}

void bst_destroy(BST *tree)
{
    BSTNode **stack;
    int top = 0;
    int capacity = 128;

    if (tree == NULL || tree->root == NULL) return;

    stack = (BSTNode **)malloc(sizeof(BSTNode *) * capacity);
    if (stack == NULL) return;

    stack[top++] = tree->root;

    while (top > 0) {
        BSTNode *node = stack[--top];

        if (node->left != NULL) {
            if (top >= capacity) {
                BSTNode **temp;
                capacity *= 2;
                temp = (BSTNode **)realloc(stack, sizeof(BSTNode *) * capacity);
                if (temp == NULL) {
                    free(stack);
                    return;
                }
                stack = temp;
            }
            stack[top++] = node->left;
        }

        if (node->right != NULL) {
            if (top >= capacity) {
                BSTNode **temp;
                capacity *= 2;
                temp = (BSTNode **)realloc(stack, sizeof(BSTNode *) * capacity);
                if (temp == NULL) {
                    free(stack);
                    return;
                }
                stack = temp;
            }
            stack[top++] = node->right;
        }

        free(node);
    }

    free(stack);
    tree->root = NULL;
    tree->build_comparisons = 0;
}
