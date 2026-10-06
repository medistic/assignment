#include "avl.h"
#include <stdlib.h>

static int node_height(const AVLNode *node)
{
    return node == NULL ? 0 : node->height;
}

static int max_int(int a, int b)
{
    return a > b ? a : b;
}

static AVLNode *create_node(int value)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (node == NULL) return NULL;
    node->data = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static int balance_factor(const AVLNode *node)
{
    if (node == NULL) return 0;
    return node_height(node->left) - node_height(node->right);
}

static AVLNode *rotate_right(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = max_int(node_height(y->left), node_height(y->right)) + 1;
    x->height = max_int(node_height(x->left), node_height(x->right)) + 1;
    return x;
}

static AVLNode *rotate_left(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = max_int(node_height(x->left), node_height(x->right)) + 1;
    y->height = max_int(node_height(y->left), node_height(y->right)) + 1;
    return y;
}

static AVLNode *insert_node(AVLTree *tree, AVLNode *node, int value, int *inserted, int *memory_error)
{
    int balance;

    if (node == NULL) {
        AVLNode *new_node = create_node(value);
        if (new_node == NULL) {
            *memory_error = 1;
            return NULL;
        }
        *inserted = 1;
        return new_node;
    }

    tree->construction_comparisons++;

    if (value == node->data) {
        *inserted = 0;
        return node;
    }

    if (value < node->data) {
        AVLNode *new_left = insert_node(tree, node->left, value, inserted, memory_error);
        if (*memory_error) return node;
        node->left = new_left;
    } else {
        AVLNode *new_right = insert_node(tree, node->right, value, inserted, memory_error);
        if (*memory_error) return node;
        node->right = new_right;
    }

    if (!*inserted) return node;

    node->height = max_int(node_height(node->left), node_height(node->right)) + 1;
    balance = balance_factor(node);

    if (balance > 1 && value < node->left->data) return rotate_right(node);          /* LL */
    if (balance < -1 && value > node->right->data) return rotate_left(node);        /* RR */
    if (balance > 1 && value > node->left->data) {                                  /* LR */
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && value < node->right->data) {                                /* RL */
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void avl_init(AVLTree *tree)
{
    if (tree == NULL) return;
    tree->root = NULL;
    tree->size = 0;
    tree->construction_comparisons = 0;
}

int avl_insert(AVLTree *tree, int value)
{
    int inserted = 0;
    int memory_error = 0;
    AVLNode *new_root;

    if (tree == NULL) return 0;

    new_root = insert_node(tree, tree->root, value, &inserted, &memory_error);
    if (memory_error) return 0;

    tree->root = new_root;
    if (inserted) tree->size++;
    return inserted;
}

int avl_search(const AVLTree *tree, int key, int *comparisons)
{
    AVLNode *current;

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

int avl_height(const AVLTree *tree)
{
    if (tree == NULL || tree->root == NULL) return 0;
    return tree->root->height;
}

static void destroy_node(AVLNode *node)
{
    if (node == NULL) return;
    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
}

void avl_destroy(AVLTree *tree)
{
    if (tree == NULL) return;
    destroy_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
    tree->construction_comparisons = 0;
}
