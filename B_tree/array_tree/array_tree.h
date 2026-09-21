#ifndef ARRAY_TREE_H
#define ARRAY_TREE_H

#include <stddef.h>

typedef struct {
    char data;
    int used;
} ArrayNode;

typedef struct {
    ArrayNode *nodes;
    size_t capacity;
    size_t max_index;
} ArrayTree;

void arrayTreeInit(ArrayTree *tree);
void arrayTreeFree(ArrayTree *tree);
int arrayTreeBuild(ArrayTree *tree, const char *text);
void arrayTreePrint(const ArrayTree *tree);
int arrayTreeNodeCount(const ArrayTree *tree);
int arrayTreeLeafCount(const ArrayTree *tree);
int arrayTreeNonLeafCount(const ArrayTree *tree);
int arrayTreeHeight(const ArrayTree *tree);
int arrayTreeDegree(const ArrayTree *tree);
int arrayTreeIsComplete(const ArrayTree *tree);
int arrayTreeIsPerfect(const ArrayTree *tree);
int arrayTreeSkewType(const ArrayTree *tree); /* -1 왼쪽, 0 아님, 1 오른쪽, 2 단일 노드 */
void arrayTreePrintRelation(const ArrayTree *tree, char data);
size_t arrayTreeMemoryBytes(const ArrayTree *tree);

#endif
