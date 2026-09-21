#include "array_tree.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *text;
    size_t pos;
    int error;
} Parser;

static void skipSpaces(Parser *p)
{
    while (isspace((unsigned char)p->text[p->pos]))
        p->pos++;
}

static int ensureCapacity(ArrayTree *tree, size_t index)
{
    size_t oldCapacity;
    size_t newCapacity;
    ArrayNode *newNodes;

    if (index < tree->capacity)
        return 1;

    oldCapacity = tree->capacity;
    newCapacity = oldCapacity ? oldCapacity : 2;
    while (index >= newCapacity) {
        if (newCapacity > ((size_t)-1) / 2)
            return 0;
        newCapacity *= 2;
    }

    newNodes = (ArrayNode *)realloc(tree->nodes, newCapacity * sizeof(*newNodes));
    if (!newNodes)
        return 0;

    tree->nodes = newNodes;
    memset(tree->nodes + oldCapacity, 0,
           (newCapacity - oldCapacity) * sizeof(*tree->nodes));
    tree->capacity = newCapacity;
    return 1;
}

static int parseNode(ArrayTree *tree, Parser *p, size_t index)
{
    char ch;

    skipSpaces(p);
    ch = p->text[p->pos];
    if (ch == '\0' || ch == ',' || ch == ')')
        return 0;
    if (ch == '(') {
        p->error = 1;
        return 0;
    }
    if (!ensureCapacity(tree, index)) {
        p->error = 2;
        return 0;
    }

    tree->nodes[index].data = ch;
    tree->nodes[index].used = 1;
    if (index > tree->max_index)
        tree->max_index = index;
    p->pos++;

    skipSpaces(p);
    if (p->text[p->pos] == '(') {
        p->pos++;
        skipSpaces(p);

        /* 공백 뒤 바로 ','이면 왼쪽 자식은 빈 노드이다. */
        if (p->text[p->pos] != ',')
            parseNode(tree, p, index * 2);
        if (p->error)
            return 0;

        skipSpaces(p);
        if (p->text[p->pos] != ',') {
            p->error = 1;
            return 0;
        }
        p->pos++;
        skipSpaces(p);

        /* 공백 뒤 바로 ')'이면 오른쪽 자식은 빈 노드이다. */
        if (p->text[p->pos] != ')')
            parseNode(tree, p, index * 2 + 1);
        if (p->error)
            return 0;

        skipSpaces(p);
        if (p->text[p->pos] != ')') {
            p->error = 1;
            return 0;
        }
        p->pos++;
    }
    return 1;
}

static int isUsed(const ArrayTree *tree, size_t index)
{
    return tree && index < tree->capacity && tree->nodes[index].used;
}

static void printRec(const ArrayTree *tree, size_t index, int level)
{
    int i;
    if (!isUsed(tree, index))
        return;
    printRec(tree, index * 2 + 1, level + 1);
    for (i = 0; i < level; i++)
        printf("    ");
    printf("%c\n", tree->nodes[index].data);
    printRec(tree, index * 2, level + 1);
}

static int heightRec(const ArrayTree *tree, size_t index)
{
    int leftHeight, rightHeight;
    if (!isUsed(tree, index))
        return 0;
    leftHeight = heightRec(tree, index * 2);
    rightHeight = heightRec(tree, index * 2 + 1);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

void arrayTreeInit(ArrayTree *tree)
{
    tree->nodes = NULL;
    tree->capacity = 0;
    tree->max_index = 0;
}

void arrayTreeFree(ArrayTree *tree)
{
    free(tree->nodes);
    arrayTreeInit(tree);
}

int arrayTreeBuild(ArrayTree *tree, const char *text)
{
    Parser p = {text, 0, 0};
    ArrayTree temp;

    if (!tree || !text)
        return 0;
    arrayTreeInit(&temp);
    if (!parseNode(&temp, &p, 1) || p.error) {
        if (p.error == 2)
            fprintf(stderr, "메모리 할당 또는 배열 인덱스 오류입니다.\n");
        else
            fprintf(stderr, "잘못된 괄호 표기법입니다.\n");
        arrayTreeFree(&temp);
        return 0;
    }
    skipSpaces(&p);
    if (p.text[p.pos] != '\0') {
        fprintf(stderr, "트리 뒤에 불필요한 문자가 있습니다.\n");
        arrayTreeFree(&temp);
        return 0;
    }
    arrayTreeFree(tree);
    *tree = temp;
    return 1;
}

void arrayTreePrint(const ArrayTree *tree)
{
    printRec(tree, 1, 0);
}

int arrayTreeNodeCount(const ArrayTree *tree)
{
    size_t i;
    int count = 0;
    if (!tree)
        return 0;
    for (i = 1; i <= tree->max_index; i++)
        if (isUsed(tree, i))
            count++;
    return count;
}

int arrayTreeLeafCount(const ArrayTree *tree)
{
    size_t i;
    int count = 0;
    if (!tree)
        return 0;
    for (i = 1; i <= tree->max_index; i++)
        if (isUsed(tree, i) && !isUsed(tree, i * 2) && !isUsed(tree, i * 2 + 1))
            count++;
    return count;
}

int arrayTreeNonLeafCount(const ArrayTree *tree)
{
    return arrayTreeNodeCount(tree) - arrayTreeLeafCount(tree);
}

int arrayTreeHeight(const ArrayTree *tree)
{
    return heightRec(tree, 1);
}

int arrayTreeDegree(const ArrayTree *tree)
{
    size_t i;
    int maxDegree = 0;
    if (!tree)
        return 0;
    for (i = 1; i <= tree->max_index; i++) {
        int degree;
        if (!isUsed(tree, i))
            continue;
        degree = isUsed(tree, i * 2) + isUsed(tree, i * 2 + 1);
        if (degree > maxDegree)
            maxDegree = degree;
    }
    return maxDegree;
}

int arrayTreeIsComplete(const ArrayTree *tree)
{
    size_t i;
    if (!isUsed(tree, 1))
        return 1;
    for (i = 1; i <= tree->max_index; i++)
        if (!isUsed(tree, i))
            return 0;
    return 1;
}

int arrayTreeIsPerfect(const ArrayTree *tree)
{
    int height = arrayTreeHeight(tree);
    size_t expected;
    if (height == 0)
        return 1;
    if ((size_t)height >= sizeof(size_t) * 8)
        return 0;
    expected = ((size_t)1 << height) - 1;
    return (size_t)arrayTreeNodeCount(tree) == expected && arrayTreeIsComplete(tree);
}

int arrayTreeSkewType(const ArrayTree *tree)
{
    size_t index = 1;
    int direction = 0;
    if (!isUsed(tree, index))
        return 0;
    while (isUsed(tree, index)) {
        int hasLeft = isUsed(tree, index * 2);
        int hasRight = isUsed(tree, index * 2 + 1);
        int currentDirection;
        if (hasLeft && hasRight)
            return 0;
        if (!hasLeft && !hasRight)
            break;
        currentDirection = hasLeft ? -1 : 1;
        if (direction != 0 && direction != currentDirection)
            return 0;
        direction = currentDirection;
        index = hasLeft ? index * 2 : index * 2 + 1;
    }
    return direction == 0 ? 2 : direction;
}

static size_t findIndex(const ArrayTree *tree, char data)
{
    size_t i;
    for (i = 1; tree && i <= tree->max_index; i++)
        if (isUsed(tree, i) && tree->nodes[i].data == data)
            return i;
    return 0;
}

static void printValue(const ArrayTree *tree, size_t index)
{
    if (isUsed(tree, index))
        printf("%c", tree->nodes[index].data);
    else
        printf("없음");
}

void arrayTreePrintRelation(const ArrayTree *tree, char data)
{
    size_t index = findIndex(tree, data);
    size_t sibling = 0;
    if (!index) {
        printf("'%c' 노드를 찾을 수 없습니다.\n", data);
        return;
    }
    printf("부모: ");
    printValue(tree, index == 1 ? 0 : index / 2);
    printf("\n왼쪽 자식: ");
    printValue(tree, index * 2);
    printf("\n오른쪽 자식: ");
    printValue(tree, index * 2 + 1);
    if (index > 1)
        sibling = (index % 2 == 0) ? index + 1 : index - 1;
    printf("\n형제: ");
    printValue(tree, sibling);
    printf("\n");
}

size_t arrayTreeMemoryBytes(const ArrayTree *tree)
{
    return tree ? sizeof(*tree) + tree->capacity * sizeof(ArrayNode) : 0;
}
