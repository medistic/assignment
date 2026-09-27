#ifndef BTREE_H
#define BTREE_H

#include <stddef.h>

typedef struct BTreeNode {
    char data;
    struct BTreeNode *left;
    struct BTreeNode *right;
} BTreeNode;

typedef struct {
    BTreeNode *root;
    size_t size;
} BTree;

/* 괄호 표기법 문자열을 연결 이진트리로 변환한다. */
int create_btree_from_string(
    const char *input,
    BTree **out_tree,
    char *error_message,
    size_t error_size
);

/* 입력된 트리의 구조를 출력한다. */
void print_btree(const BTree *tree);

/* 세 순회는 모두 반복적(iterative) 방법으로 구현된다. */
void preorder(const BTree *tree);
void inorder(const BTree *tree);
void postorder(const BTree *tree);

/* 트리의 모든 노드와 트리 객체를 제거한다. */
void destroy_btree(BTree *tree);

#endif
