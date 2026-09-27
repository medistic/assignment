#include "btree.h"

#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 2048
#define ERROR_SIZE 256

int main(void)
{
    char input[INPUT_SIZE];
    char error_message[ERROR_SIZE];
    BTree *tree = NULL;

    printf("Input binary tree: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error: failed to read input.\n");
        return 1;
    }

    input[strcspn(input, "\r\n")] = '\0';

    if (!create_btree_from_string(
            input,
            &tree,
            error_message,
            sizeof(error_message)))
    {
        printf("%s\n", error_message);
        return 1;
    }

    printf("\nBinary Tree\n");
    print_btree(tree);

    printf("\nPreorder  : ");
    preorder(tree);

    printf("Inorder   : ");
    inorder(tree);

    printf("Postorder : ");
    postorder(tree);

    destroy_btree(tree);

    return 0;
}
