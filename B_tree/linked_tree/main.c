#include "linked_tree.h"

#include <stdio.h>

#define INPUT_SIZE 2048

static const char *yesNo(int value)
{
    return value ? "예" : "아니오";
}

static const char *skewName(int type)
{
    if (type == -1) return "예 (왼쪽 편향)";
    if (type == 1) return "예 (오른쪽 편향)";
    if (type == 2) return "예 (단일 노드)";
    return "아니오";
}

int main(void)
{
    char input[INPUT_SIZE];
    char target;
    LinkedTree tree;

    linkedTreeInit(&tree);
    printf("괄호 표기법 입력 (예: a(b(d,e),c( ,f))):\n> ");
    if (!fgets(input, sizeof(input), stdin) || !linkedTreeBuild(&tree, input)) {
        linkedTreeFree(&tree);
        return 1;
    }

    printf("\n[연결 이진트리]\n");
    linkedTreePrint(&tree);
    printf("\n전체 노드 수: %d\n", linkedTreeNodeCount(&tree));
    printf("단말 노드 수: %d\n", linkedTreeLeafCount(&tree));
    printf("비단말 노드 수: %d\n", linkedTreeNonLeafCount(&tree));
    printf("높이(루트=1): %d\n", linkedTreeHeight(&tree));
    printf("차수: %d\n", linkedTreeDegree(&tree));
    printf("완전 이진트리: %s\n", yesNo(linkedTreeIsComplete(&tree)));
    printf("포화 이진트리: %s\n", yesNo(linkedTreeIsPerfect(&tree)));
    printf("편향 이진트리: %s\n", skewName(linkedTreeSkewType(&tree)));
    printf("메모리 사용량(관리 구조체 포함): %zu bytes\n",
           linkedTreeMemoryBytes(&tree));

    printf("\n관계를 조회할 노드 한 글자: ");
    if (scanf(" %c", &target) == 1)
        linkedTreePrintRelation(&tree, target);

    linkedTreeFree(&tree);
    return 0;
}
