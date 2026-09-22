#ifndef BTREE_H
#define BTREE_H

#include <stddef.h>

typedef struct BTreeNode {
    char data;
    struct BTreeNode *left;
    struct BTreeNode *right;
    struct BTreeNode *parent;
} BTreeNode;

typedef struct {
    BTreeNode *root;
    size_t count;
    size_t max_size;
} BTree;

typedef enum {
    BTREE_OK = 0,
    BTREE_ERR_FULL,
    BTREE_ERR_NOT_EMPTY,
    BTREE_ERR_INVALID_VALUE,
    BTREE_ERR_NOT_FOUND,
    BTREE_ERR_NOT_LEAF,
    BTREE_ERR_INVALID_CHILD,
    BTREE_ERR_POSITION_OCCUPIED,
    BTREE_ERR_DUPLICATE_SIBLING
} BTreeResult;

BTree *create_btree(size_t size);
BTreeResult insert_root(BTree *tree, char value);
BTreeResult insert_child(BTree *tree, BTreeNode *parent, char child, char value);
BTreeResult delete_node(BTree *tree, BTreeNode *leaf);
BTreeResult update_value(BTree *tree, BTreeNode *node, char value);
void read_child(const BTree *tree, const BTreeNode *parent);
void print_btree(const BTree *tree);
void destroy_btree(BTree *tree);

BTreeNode *find_node_by_path(const BTree *tree, const char *path);
int is_leaf(const BTreeNode *node);

#endif
