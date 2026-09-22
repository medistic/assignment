#include "btree.h"

#include <stdio.h>
#include <stdlib.h>

static int valid_value(char value)
{
    return value >= 'A' && value <= 'Z';
}

static BTreeNode *create_node(char value, BTreeNode *parent)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));

    if (node == NULL)
        return NULL;

    node->data = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = parent;

    return node;
}

BTree *create_btree(size_t size)
{
    BTree *tree;

    if (size == 0)
        return NULL;

    tree = (BTree *)malloc(sizeof(BTree));

    if (tree == NULL)
        return NULL;

    tree->root = NULL;
    tree->count = 0;
    tree->max_size = size;

    return tree;
}

int is_leaf(const BTreeNode *node)
{
    return node != NULL &&
           node->left == NULL &&
           node->right == NULL;
}

BTreeResult insert_root(BTree *tree, char value)
{
    if (tree == NULL)
        return BTREE_ERR_NOT_FOUND;

    if (!valid_value(value))
        return BTREE_ERR_INVALID_VALUE;

    if (tree->root != NULL)
        return BTREE_ERR_NOT_EMPTY;

    if (tree->count >= tree->max_size)
        return BTREE_ERR_FULL;

    tree->root = create_node(value, NULL);

    if (tree->root == NULL)
        return BTREE_ERR_FULL;

    tree->count = 1;

    return BTREE_OK;
}

BTreeResult insert_child(
    BTree *tree,
    BTreeNode *parent,
    char child,
    char value
)
{
    BTreeNode **target;
    BTreeNode *sibling;

    if (tree == NULL || parent == NULL)
        return BTREE_ERR_NOT_FOUND;

    if (!valid_value(value))
        return BTREE_ERR_INVALID_VALUE;

    if (child != 'L' && child != 'R')
        return BTREE_ERR_INVALID_CHILD;

    if (tree->count >= tree->max_size)
        return BTREE_ERR_FULL;

    /*
     * 과제 명세에 따라
     * 부모가 단말 노드인 경우에만 삽입 허용
     */
    if (!is_leaf(parent))
        return BTREE_ERR_NOT_LEAF;

    if (child == 'L') {
        target = &parent->left;
        sibling = parent->right;
    }
    else {
        target = &parent->right;
        sibling = parent->left;
    }

    if (*target != NULL)
        return BTREE_ERR_POSITION_OCCUPIED;

    if (sibling != NULL && sibling->data == value)
        return BTREE_ERR_DUPLICATE_SIBLING;

    *target = create_node(value, parent);

    if (*target == NULL)
        return BTREE_ERR_FULL;

    tree->count++;

    return BTREE_OK;
}

BTreeResult delete_node(BTree *tree, BTreeNode *leaf)
{
    if (tree == NULL || leaf == NULL)
        return BTREE_ERR_NOT_FOUND;

    if (!is_leaf(leaf))
        return BTREE_ERR_NOT_LEAF;

    if (leaf == tree->root) {
        free(leaf);
        tree->root = NULL;
        tree->count = 0;

        return BTREE_OK;
    }

    if (leaf->parent->left == leaf)
        leaf->parent->left = NULL;
    else if (leaf->parent->right == leaf)
        leaf->parent->right = NULL;
    else
        return BTREE_ERR_NOT_FOUND;

    free(leaf);
    tree->count--;

    return BTREE_OK;
}

BTreeResult update_value(BTree *tree, BTreeNode *node, char value)
{
    BTreeNode *sibling = NULL;

    if (tree == NULL || node == NULL)
        return BTREE_ERR_NOT_FOUND;

    if (!valid_value(value))
        return BTREE_ERR_INVALID_VALUE;

    if (node->parent != NULL) {
        if (node->parent->left == node)
            sibling = node->parent->right;
        else if (node->parent->right == node)
            sibling = node->parent->left;

        if (sibling != NULL && sibling->data == value)
            return BTREE_ERR_DUPLICATE_SIBLING;
    }

    node->data = value;

    return BTREE_OK;
}

void read_child(const BTree *tree, const BTreeNode *parent)
{
    (void)tree;

    if (parent == NULL)
        return;

    if (parent->left == NULL && parent->right == NULL) {
        printf("No child\n");
        return;
    }

    if (parent->left != NULL && parent->right != NULL) {
        printf(
            "%c(L), %c(R)\n",
            parent->left->data,
            parent->right->data
        );
    }
    else if (parent->left != NULL) {
        printf("%c(L)\n", parent->left->data);
    }
    else {
        printf("%c(R)\n", parent->right->data);
    }
}

static void print_subtree(const BTreeNode *node, int depth)
{
    int i;

    if (node == NULL)
        return;

    for (i = 0; i < depth; i++)
        printf("    ");

    printf("+---%c\n", node->data);

    print_subtree(node->left, depth + 1);
    print_subtree(node->right, depth + 1);
}

void print_btree(const BTree *tree)
{
    if (tree == NULL || tree->root == NULL) {
        printf("Tree is empty.\n");
        return;
    }

    printf("%c\n", tree->root->data);

    print_subtree(tree->root->left, 1);
    print_subtree(tree->root->right, 1);
}

static void destroy_nodes(BTreeNode *node)
{
    if (node == NULL)
        return;

    destroy_nodes(node->left);
    destroy_nodes(node->right);

    free(node);
}

void destroy_btree(BTree *tree)
{
    if (tree == NULL)
        return;

    destroy_nodes(tree->root);
    free(tree);
}

BTreeNode *find_node_by_path(const BTree *tree, const char *path)
{
    const char *p;
    BTreeNode *current;

    if (tree == NULL || tree->root == NULL || path == NULL)
        return NULL;

    if (path[0] != '/')
        return NULL;

    p = path + 1;

    if (*p == '\0')
        return NULL;

    if (!valid_value(*p))
        return NULL;

    if (*p != tree->root->data)
        return NULL;

    current = tree->root;
    p++;

    while (*p != '\0') {
        char value;

        if (*p != '/')
            return NULL;

        p++;

        if (!valid_value(*p))
            return NULL;

        value = *p;
        p++;

        if (*p != '\0' && *p != '/')
            return NULL;

        if (current->left != NULL &&
            current->left->data == value)
        {
            current = current->left;
        }
        else if (current->right != NULL &&
                 current->right->data == value)
        {
            current = current->right;
        }
        else {
            return NULL;
        }
    }

    return current;
}
