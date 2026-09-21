#include "linked_tree.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char *text;
    size_t pos;
    int error;
} Parser;

typedef struct {
    const TreeNode **items;
    size_t front;
    size_t rear;
    size_t capacity;
} NodeQueue;

static void skipSpaces(Parser *p)
{
    while (isspace((unsigned char)p->text[p->pos]))
        p->pos++;
}

static void freeNode(TreeNode *node)
{
    if (!node)
        return;
    freeNode(node->left);
    freeNode(node->right);
    free(node);
}

static TreeNode *parseNode(Parser *p)
{
    TreeNode *node;
    char ch;

    skipSpaces(p);
    ch = p->text[p->pos];
    if (ch == '\0' || ch == ',' || ch == ')')
        return NULL;
    if (ch == '(') {
        p->error = 1;
        return NULL;
    }

    node = (TreeNode *)calloc(1, sizeof(*node));
    if (!node) {
        p->error = 2;
        return NULL;
    }
    node->data = ch;
    p->pos++;

    skipSpaces(p);
    if (p->text[p->pos] == '(') {
        p->pos++;
        skipSpaces(p);
        if (p->text[p->pos] != ',')
            node->left = parseNode(p);
        if (p->error)
            goto fail;

        skipSpaces(p);
        if (p->text[p->pos] != ',') {
            p->error = 1;
            goto fail;
        }
        p->pos++;
        skipSpaces(p);
        if (p->text[p->pos] != ')')
            node->right = parseNode(p);
        if (p->error)
            goto fail;

        skipSpaces(p);
        if (p->text[p->pos] != ')') {
            p->error = 1;
            goto fail;
        }
        p->pos++;
    }
    return node;

fail:
    freeNode(node);
    return NULL;
}

static void printRec(const TreeNode *node, int level)
{
    int i;
    if (!node)
        return;
    printRec(node->right, level + 1);
    for (i = 0; i < level; i++)
        printf("    ");
    printf("%c\n", node->data);
    printRec(node->left, level + 1);
}

static int countRec(const TreeNode *node)
{
    return node ? 1 + countRec(node->left) + countRec(node->right) : 0;
}

static int leafRec(const TreeNode *node)
{
    if (!node)
        return 0;
    if (!node->left && !node->right)
        return 1;
    return leafRec(node->left) + leafRec(node->right);
}

static int heightRec(const TreeNode *node)
{
    int leftHeight, rightHeight;
    if (!node)
        return 0;
    leftHeight = heightRec(node->left);
    rightHeight = heightRec(node->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

static int degreeRec(const TreeNode *node)
{
    int here, leftDegree, rightDegree, max;
    if (!node)
        return 0;
    here = (node->left != NULL) + (node->right != NULL);
    leftDegree = degreeRec(node->left);
    rightDegree = degreeRec(node->right);
    max = here > leftDegree ? here : leftDegree;
    return max > rightDegree ? max : rightDegree;
}

static int perfectRec(const TreeNode *node, int depth, int leafDepth)
{
    if (!node)
        return 1;
    if (!node->left && !node->right)
        return depth == leafDepth;
    if (!node->left || !node->right)
        return 0;
    return perfectRec(node->left, depth + 1, leafDepth) &&
           perfectRec(node->right, depth + 1, leafDepth);
}

static int queuePush(NodeQueue *q, const TreeNode *node)
{
    const TreeNode **newItems;
    size_t newCapacity;
    if (q->rear < q->capacity) {
        q->items[q->rear++] = node;
        return 1;
    }
    newCapacity = q->capacity ? q->capacity * 2 : 16;
    newItems = (const TreeNode **)realloc(q->items,
                                          newCapacity * sizeof(*newItems));
    if (!newItems)
        return 0;
    q->items = newItems;
    q->capacity = newCapacity;
    q->items[q->rear++] = node;
    return 1;
}

static const TreeNode *findWithParent(const TreeNode *node, char data,
                                      const TreeNode **parent)
{
    const TreeNode *found;
    if (!node)
        return NULL;
    if (node->data == data)
        return node;
    if (node->left) {
        *parent = node;
        found = findWithParent(node->left, data, parent);
        if (found)
            return found;
    }
    if (node->right) {
        *parent = node;
        found = findWithParent(node->right, data, parent);
        if (found)
            return found;
    }
    return NULL;
}

void linkedTreeInit(LinkedTree *tree)
{
    tree->root = NULL;
}

void linkedTreeFree(LinkedTree *tree)
{
    if (!tree)
        return;
    freeNode(tree->root);
    tree->root = NULL;
}

int linkedTreeBuild(LinkedTree *tree, const char *text)
{
    Parser p = {text, 0, 0};
    TreeNode *root;
    if (!tree || !text)
        return 0;
    root = parseNode(&p);
    if (!root || p.error) {
        if (p.error == 2)
            fprintf(stderr, "메모리 할당 오류입니다.\n");
        else
            fprintf(stderr, "잘못된 괄호 표기법입니다.\n");
        freeNode(root);
        return 0;
    }
    skipSpaces(&p);
    if (p.text[p.pos] != '\0') {
        fprintf(stderr, "트리 뒤에 불필요한 문자가 있습니다.\n");
        freeNode(root);
        return 0;
    }
    linkedTreeFree(tree);
    tree->root = root;
    return 1;
}

void linkedTreePrint(const LinkedTree *tree)
{
    if (tree)
        printRec(tree->root, 0);
}

int linkedTreeNodeCount(const LinkedTree *tree)
{
    return tree ? countRec(tree->root) : 0;
}

int linkedTreeLeafCount(const LinkedTree *tree)
{
    return tree ? leafRec(tree->root) : 0;
}

int linkedTreeNonLeafCount(const LinkedTree *tree)
{
    return linkedTreeNodeCount(tree) - linkedTreeLeafCount(tree);
}

int linkedTreeHeight(const LinkedTree *tree)
{
    return tree ? heightRec(tree->root) : 0;
}

int linkedTreeDegree(const LinkedTree *tree)
{
    return tree ? degreeRec(tree->root) : 0;
}

int linkedTreeIsComplete(const LinkedTree *tree)
{
    NodeQueue q = {NULL, 0, 0, 0};
    int sawEmpty = 0;
    int result = 1;
    if (!tree || !tree->root)
        return 1;
    if (!queuePush(&q, tree->root))
        return 0;
    while (q.front < q.rear) {
        const TreeNode *node = q.items[q.front++];
        if (!node) {
            sawEmpty = 1;
            continue;
        }
        if (sawEmpty) {
            result = 0;
            break;
        }
        if (!queuePush(&q, node->left) || !queuePush(&q, node->right)) {
            result = 0;
            break;
        }
    }
    free(q.items);
    return result;
}

int linkedTreeIsPerfect(const LinkedTree *tree)
{
    int height = linkedTreeHeight(tree);
    return !tree || !tree->root || perfectRec(tree->root, 1, height);
}

int linkedTreeSkewType(const LinkedTree *tree)
{
    const TreeNode *node;
    int direction = 0;
    if (!tree || !tree->root)
        return 0;
    node = tree->root;
    while (node) {
        int currentDirection;
        if (node->left && node->right)
            return 0;
        if (!node->left && !node->right)
            break;
        currentDirection = node->left ? -1 : 1;
        if (direction != 0 && direction != currentDirection)
            return 0;
        direction = currentDirection;
        node = node->left ? node->left : node->right;
    }
    return direction == 0 ? 2 : direction;
}

static void printNodeValue(const TreeNode *node)
{
    if (node)
        printf("%c", node->data);
    else
        printf("없음");
}

void linkedTreePrintRelation(const LinkedTree *tree, char data)
{
    const TreeNode *parent = NULL;
    const TreeNode *target;
    const TreeNode *sibling = NULL;
    if (!tree)
        return;
    target = findWithParent(tree->root, data, &parent);
    if (!target) {
        printf("'%c' 노드를 찾을 수 없습니다.\n", data);
        return;
    }
    if (parent)
        sibling = parent->left == target ? parent->right : parent->left;
    printf("부모: ");
    printNodeValue(parent);
    printf("\n왼쪽 자식: ");
    printNodeValue(target->left);
    printf("\n오른쪽 자식: ");
    printNodeValue(target->right);
    printf("\n형제: ");
    printNodeValue(sibling);
    printf("\n");
}

size_t linkedTreeMemoryBytes(const LinkedTree *tree)
{
    return sizeof(LinkedTree) +
           (size_t)linkedTreeNodeCount(tree) * sizeof(TreeNode);
}
