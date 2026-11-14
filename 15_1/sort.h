#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include <stdint.h>

typedef long long Count;

typedef struct {
    int type;   /* 0 = insert, 1 = delete */
    int value;
} Operation;

typedef struct {
    Count comparisons;
    Count movements;
    size_t memory_bytes;
    int final_count;
    int max_depth;
} MethodStats;

typedef struct AVLNode {
    int key;
    int count;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct {
    AVLNode *root;
    int total_count;
    int unique_count;
    int peak_unique_count;
    int max_operation_depth;
    Count comparisons;
    Count movements;
} AVLTree;

void quick_array_process(
    int data[],
    int n,
    const Operation operations[],
    int operation_count,
    MethodStats *stats,
    int *output_count
);

int avl_process(
    const int original[],
    int n,
    const Operation operations[],
    int operation_count,
    MethodStats *stats,
    AVLTree *tree
);

int avl_matches_sorted_array(
    const AVLTree *tree,
    const int array[],
    int count
);

void avl_destroy(AVLTree *tree);

#endif
