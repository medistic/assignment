#ifndef LINKED_TREE_H
#define LINKED_TREE_H

#include <stddef.h>

typedef struct TreeNode {
    char data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    TreeNode *root;
} LinkedTree;

void linkedTreeInit(LinkedTree *tree);
void linkedTreeFree(LinkedTree *tree);
int linkedTreeBuild(LinkedTree *tree, const char *text);
void linkedTreePrint(const LinkedTree *tree);
int linkedTreeNodeCount(const LinkedTree *tree);
int linkedTreeLeafCount(const LinkedTree *tree);
int linkedTreeNonLeafCount(const LinkedTree *tree);
int linkedTreeHeight(const LinkedTree *tree);
int linkedTreeDegree(const LinkedTree *tree);
int linkedTreeIsComplete(const LinkedTree *tree);
int linkedTreeIsPerfect(const LinkedTree *tree);
int linkedTreeSkewType(const LinkedTree *tree); /* -1 왼쪽, 0 아님, 1 오른쪽, 2 단일 노드 */
void linkedTreePrintRelation(const LinkedTree *tree, char data);
size_t linkedTreeMemoryBytes(const LinkedTree *tree);

#endif
