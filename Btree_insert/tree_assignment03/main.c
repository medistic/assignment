#include "btree.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_NODES 100
#define INPUT_SIZE 512
#define MAX_TOKENS 16

static int equals_ignore_case(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) !=
            tolower((unsigned char)*b))
        {
            return 0;
        }

        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

static int is_command(
    const char *token,
    const char *short_name,
    const char *full_name
)
{
    return equals_ignore_case(token, short_name) ||
           equals_ignore_case(token, full_name);
}

static int parse_value(const char *token, char *value)
{
    if (token == NULL ||
        token[0] == '\0' ||
        token[1] != '\0')
    {
        return 0;
    }

    if (token[0] < 'A' || token[0] > 'Z')
        return 0;

    *value = token[0];

    return 1;
}

static int parse_child(const char *token, char *child)
{
    if (equals_ignore_case(token, "L") ||
        equals_ignore_case(token, "Left"))
    {
        *child = 'L';
        return 1;
    }

    if (equals_ignore_case(token, "R") ||
        equals_ignore_case(token, "Right"))
    {
        *child = 'R';
        return 1;
    }

    return 0;
}

static void print_result_error(BTreeResult result)
{
    switch (result) {
    case BTREE_ERR_FULL:
        printf("Error: tree is full.\n");
        break;

    case BTREE_ERR_NOT_EMPTY:
        printf("Error: root already exists.\n");
        break;

    case BTREE_ERR_INVALID_VALUE:
        printf("Error: data must be one uppercase letter.\n");
        break;

    case BTREE_ERR_NOT_FOUND:
        printf("Error: node does not exist.\n");
        break;

    case BTREE_ERR_NOT_LEAF:
        printf("Error: target node is not a leaf node.\n");
        break;

    case BTREE_ERR_INVALID_CHILD:
        printf("Error: child must be L or R.\n");
        break;

    case BTREE_ERR_POSITION_OCCUPIED:
        printf("Error: child position is already occupied.\n");
        break;

    case BTREE_ERR_DUPLICATE_SIBLING:
        printf(
            "Error: left and right children "
            "cannot have the same data.\n"
        );
        break;

    case BTREE_OK:
    default:
        break;
    }
}

int main(void)
{
    BTree *tree;
    char line[INPUT_SIZE];

    tree = create_btree(MAX_NODES);

    if (tree == NULL) {
        fprintf(stderr, "Error: failed to create tree.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), stdin) != NULL) {
        char *argv[MAX_TOKENS];
        int argc = 0;
        char *token;

        token = strtok(line, " \t\r\n");

        while (token != NULL && argc < MAX_TOKENS) {
            argv[argc++] = token;
            token = strtok(NULL, " \t\r\n");
        }

        if (token != NULL) {
            printf("Error: too many arguments.\n");
            continue;
        }

        if (argc == 0)
            continue;

        if (is_command(argv[0], "I", "Insert")) {

            if (argc == 3 && strcmp(argv[1], "/") == 0) {
                char value;
                BTreeResult result;

                if (!parse_value(argv[2], &value)) {
                    printf(
                        "Error: data must be "
                        "one uppercase letter.\n"
                    );
                    continue;
                }

                result = insert_root(tree, value);
                print_result_error(result);

                continue;
            }

            if (argc == 4) {
                BTreeNode *parent;
                char child;
                char value;
                BTreeResult result;

                parent = find_node_by_path(tree, argv[1]);

                if (parent == NULL) {
                    printf(
                        "Error: parent node "
                        "does not exist.\n"
                    );
                    continue;
                }

                if (!parse_child(argv[2], &child)) {
                    printf("Error: child must be L or R.\n");
                    continue;
                }

                if (!parse_value(argv[3], &value)) {
                    printf(
                        "Error: data must be "
                        "one uppercase letter.\n"
                    );
                    continue;
                }

                result = insert_child(
                    tree,
                    parent,
                    child,
                    value
                );

                print_result_error(result);

                continue;
            }

            printf("Error: invalid Insert command.\n");
        }

        else if (is_command(argv[0], "D", "Delete")) {
            BTreeNode *leaf;
            BTreeResult result;

            if (argc != 2) {
                printf("Error: invalid Delete command.\n");
                continue;
            }

            leaf = find_node_by_path(tree, argv[1]);

            if (leaf == NULL) {
                printf("Error: node does not exist.\n");
                continue;
            }

            result = delete_node(tree, leaf);
            print_result_error(result);
        }

        else if (is_command(argv[0], "U", "Update")) {
            BTreeNode *node;
            char value;
            BTreeResult result;

            if (argc != 3) {
                printf("Error: invalid Update command.\n");
                continue;
            }

            node = find_node_by_path(tree, argv[1]);

            if (node == NULL) {
                printf("Error: node does not exist.\n");
                continue;
            }

            if (!parse_value(argv[2], &value)) {
                printf(
                    "Error: data must be "
                    "one uppercase letter.\n"
                );
                continue;
            }

            result = update_value(tree, node, value);
            print_result_error(result);
        }

        else if (is_command(argv[0], "R", "Read")) {
            BTreeNode *parent;

            if (argc != 2) {
                printf("Error: invalid Read command.\n");
                continue;
            }

            parent = find_node_by_path(tree, argv[1]);

            if (parent == NULL) {
                printf("Error: node does not exist.\n");
                continue;
            }

            read_child(tree, parent);
        }

        else if (is_command(argv[0], "P", "Print")) {
            if (argc != 1) {
                printf("Error: invalid Print command.\n");
                continue;
            }

            print_btree(tree);
        }

        else {
            printf("Error: invalid command.\n");
        }
    }

    destroy_btree(tree);

    return 0;
}
