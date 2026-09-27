#include "btree.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    BTreeNode *node;
    int stage;
} ParseFrame;

typedef struct {
    BTreeNode *node;
    size_t depth;
} PrintFrame;

static BTreeNode *create_node(char data)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));

    if (node == NULL)
        return NULL;

    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static void set_error(char *message, size_t size, const char *text)
{
    if (message == NULL || size == 0)
        return;

    snprintf(message, size, "%s", text);
}

/*
 * 괄호 표기법 예:
 *   A(B(D,E),C(,F))
 *   A( ,B)
 *   A(B, )
 *
 * 공백은 무시한다.
 * 왼쪽 자식이 없으면 '('와 ',' 사이를 비운다.
 * 오른쪽 자식이 없으면 ','와 ')' 사이를 비운다.
 *
 * 파서 역시 재귀를 사용하지 않고 명시적 스택으로 동작한다.
 */
int create_btree_from_string(
    const char *input,
    BTree **out_tree,
    char *error_message,
    size_t error_size
)
{
    BTree *tree = NULL;
    ParseFrame *stack = NULL;
    size_t stack_capacity;
    size_t top = 0;
    size_t i = 0;
    BTreeNode *last_node = NULL;
    int can_open_children = 0;

    if (out_tree == NULL) {
        set_error(error_message, error_size, "Error: invalid output pointer.");
        return 0;
    }

    *out_tree = NULL;

    if (input == NULL) {
        set_error(error_message, error_size, "Error: input is empty.");
        return 0;
    }

    while (isspace((unsigned char)input[i]))
        i++;

    if (input[i] == '\0') {
        set_error(error_message, error_size, "Error: input is empty.");
        return 0;
    }

    if (!isalpha((unsigned char)input[i])) {
        set_error(error_message, error_size,
                  "Error: the root must be an alphabetic character.");
        return 0;
    }

    tree = (BTree *)malloc(sizeof(BTree));
    if (tree == NULL) {
        set_error(error_message, error_size, "Error: memory allocation failed.");
        return 0;
    }

    tree->root = create_node(input[i]);
    if (tree->root == NULL) {
        free(tree);
        set_error(error_message, error_size, "Error: memory allocation failed.");
        return 0;
    }

    tree->size = 1;
    last_node = tree->root;
    can_open_children = 1;
    i++;

    stack_capacity = strlen(input) + 1;
    stack = (ParseFrame *)malloc(sizeof(ParseFrame) * stack_capacity);
    if (stack == NULL) {
        destroy_btree(tree);
        set_error(error_message, error_size, "Error: memory allocation failed.");
        return 0;
    }

    while (input[i] != '\0') {
        char ch = input[i];

        if (isspace((unsigned char)ch)) {
            i++;
            continue;
        }

        if (isalpha((unsigned char)ch)) {
            BTreeNode *new_node;
            ParseFrame *frame;

            if (top == 0) {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: unexpected node after the root.");
                return 0;
            }

            frame = &stack[top - 1];

            if (frame->stage != 0 && frame->stage != 2) {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: missing comma or closing parenthesis.");
                return 0;
            }

            new_node = create_node(ch);
            if (new_node == NULL) {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: memory allocation failed.");
                return 0;
            }

            if (frame->stage == 0) {
                frame->node->left = new_node;
                frame->stage = 1;
            }
            else {
                frame->node->right = new_node;
                frame->stage = 3;
            }

            tree->size++;
            last_node = new_node;
            can_open_children = 1;
            i++;
            continue;
        }

        if (ch == '(') {
            if (!can_open_children || last_node == NULL) {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: '(' must immediately follow a node.");
                return 0;
            }

            stack[top].node = last_node;
            stack[top].stage = 0;
            top++;

            can_open_children = 0;
            i++;
            continue;
        }

        if (ch == ',') {
            ParseFrame *frame;

            if (top == 0) {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: comma is outside a child list.");
                return 0;
            }

            frame = &stack[top - 1];

            if (frame->stage == 0 || frame->stage == 1) {
                frame->stage = 2;
            }
            else {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: too many commas in one child list.");
                return 0;
            }

            can_open_children = 0;
            i++;
            continue;
        }

        if (ch == ')') {
            ParseFrame *frame;

            if (top == 0) {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: unmatched closing parenthesis.");
                return 0;
            }

            frame = &stack[top - 1];

            if (frame->stage != 2 && frame->stage != 3) {
                free(stack);
                destroy_btree(tree);
                set_error(error_message, error_size,
                          "Error: each child list must contain one comma.");
                return 0;
            }

            top--;
            can_open_children = 0;
            i++;
            continue;
        }

        free(stack);
        destroy_btree(tree);
        set_error(error_message, error_size,
                  "Error: only alphabetic characters, '(', ')', ',' and spaces are allowed.");
        return 0;
    }

    if (top != 0) {
        free(stack);
        destroy_btree(tree);
        set_error(error_message, error_size,
                  "Error: unmatched opening parenthesis.");
        return 0;
    }

    free(stack);
    *out_tree = tree;
    return 1;
}

void print_btree(const BTree *tree)
{
    PrintFrame *stack;
    size_t top = 0;

    if (tree == NULL || tree->root == NULL) {
        printf("Tree is empty.\n");
        return;
    }

    stack = (PrintFrame *)malloc(sizeof(PrintFrame) * tree->size);
    if (stack == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    printf("%c\n", tree->root->data);

    if (tree->root->right != NULL) {
        stack[top].node = tree->root->right;
        stack[top].depth = 1;
        top++;
    }

    if (tree->root->left != NULL) {
        stack[top].node = tree->root->left;
        stack[top].depth = 1;
        top++;
    }

    while (top > 0) {
        PrintFrame frame = stack[--top];
        size_t i;

        for (i = 0; i < frame.depth; i++)
            printf("    ");

        printf("+---%c\n", frame.node->data);

        if (frame.node->right != NULL) {
            stack[top].node = frame.node->right;
            stack[top].depth = frame.depth + 1;
            top++;
        }

        if (frame.node->left != NULL) {
            stack[top].node = frame.node->left;
            stack[top].depth = frame.depth + 1;
            top++;
        }
    }

    free(stack);
}

void preorder(const BTree *tree)
{
    BTreeNode **stack;
    size_t top = 0;
    int first = 1;

    if (tree == NULL || tree->root == NULL) {
        printf("(empty)\n");
        return;
    }

    stack = (BTreeNode **)malloc(sizeof(BTreeNode *) * tree->size);
    if (stack == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    stack[top++] = tree->root;

    while (top > 0) {
        BTreeNode *node = stack[--top];

        if (!first)
            printf(" ");
        printf("%c", node->data);
        first = 0;

        if (node->right != NULL)
            stack[top++] = node->right;

        if (node->left != NULL)
            stack[top++] = node->left;
    }

    printf("\n");
    free(stack);
}

void inorder(const BTree *tree)
{
    BTreeNode **stack;
    size_t top = 0;
    BTreeNode *current;
    int first = 1;

    if (tree == NULL || tree->root == NULL) {
        printf("(empty)\n");
        return;
    }

    stack = (BTreeNode **)malloc(sizeof(BTreeNode *) * tree->size);
    if (stack == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    current = tree->root;

    while (current != NULL || top > 0) {
        while (current != NULL) {
            stack[top++] = current;
            current = current->left;
        }

        current = stack[--top];

        if (!first)
            printf(" ");
        printf("%c", current->data);
        first = 0;

        current = current->right;
    }

    printf("\n");
    free(stack);
}

void postorder(const BTree *tree)
{
    BTreeNode **stack;
    size_t top = 0;
    BTreeNode *current;
    BTreeNode *last_visited = NULL;
    int first = 1;

    if (tree == NULL || tree->root == NULL) {
        printf("(empty)\n");
        return;
    }

    stack = (BTreeNode **)malloc(sizeof(BTreeNode *) * tree->size);
    if (stack == NULL) {
        printf("Error: memory allocation failed.\n");
        return;
    }

    current = tree->root;

    while (current != NULL || top > 0) {
        if (current != NULL) {
            stack[top++] = current;
            current = current->left;
        }
        else {
            BTreeNode *peek = stack[top - 1];

            if (peek->right != NULL && last_visited != peek->right) {
                current = peek->right;
            }
            else {
                if (!first)
                    printf(" ");
                printf("%c", peek->data);
                first = 0;

                last_visited = peek;
                top--;
            }
        }
    }

    printf("\n");
    free(stack);
}

void destroy_btree(BTree *tree)
{
    BTreeNode **stack;
    size_t top = 0;

    if (tree == NULL)
        return;

    if (tree->root == NULL) {
        free(tree);
        return;
    }

    stack = (BTreeNode **)malloc(sizeof(BTreeNode *) * tree->size);

    if (stack == NULL) {
        /* 메모리 부족 상황에서는 트리 객체만 제거할 수 있다. */
        free(tree);
        return;
    }

    stack[top++] = tree->root;

    while (top > 0) {
        BTreeNode *node = stack[--top];

        if (node->left != NULL)
            stack[top++] = node->left;

        if (node->right != NULL)
            stack[top++] = node->right;

        free(node);
    }

    free(stack);
    free(tree);
}
