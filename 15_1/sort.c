#include "sort.h"

#include <stdlib.h>
#include <string.h>

/*
 * Actual machine stack-frame size depends on ABI/compiler options.
 * For reproducible comparison, stack memory is modeled by explicit
 * logical frame structures containing the state needed by each recursion.
 */
typedef struct {
    int low;
    int high;
    int pivot;
    int *data;
    Count *comparisons;
    Count *movements;
} QuickFrameEstimate;

typedef struct {
    AVLNode *node;
    int key;
    Count *comparisons;
    Count *movements;
    int depth;
} AVLFrameEstimate;

/* ========================= QUICK SORT + SORTED ARRAY ========================= */

static void swap_counted(int *a, int *b, Count *movements)
{
    int temp;

    if (a == b) {
        return;
    }

    temp = *a;      /* movement 1 */
    *a = *b;        /* movement 2 */
    *b = temp;      /* movement 3 */
    *movements += 3;
}

static int median3_index(
    const int data[],
    int low,
    int high,
    Count *comparisons,
    Count *movements
)
{
    int middle = low + (high - low) / 2;

    /* Copies of data values into temporary variables are counted as moves. */
    int a = data[low];
    int b = data[middle];
    int c = data[high];
    *movements += 3;

    (*comparisons)++;
    if (a < b) {
        (*comparisons)++;
        if (b < c) {
            return middle;
        }

        (*comparisons)++;
        if (a < c) {
            return high;
        }

        return low;
    }
    else {
        (*comparisons)++;
        if (a < c) {
            return low;
        }

        (*comparisons)++;
        if (b < c) {
            return high;
        }

        return middle;
    }
}

static int partition_lomuto(
    int data[],
    int low,
    int high,
    Count *comparisons,
    Count *movements
)
{
    int pivot_index = median3_index(
        data,
        low,
        high,
        comparisons,
        movements
    );

    int pivot_value;
    int i;
    int j;

    swap_counted(
        &data[pivot_index],
        &data[high],
        movements
    );

    pivot_value = data[high];
    (*movements)++;

    i = low - 1;

    for (j = low; j < high; j++) {
        (*comparisons)++;

        if (data[j] <= pivot_value) {
            i++;
            swap_counted(
                &data[i],
                &data[j],
                movements
            );
        }
    }

    swap_counted(
        &data[i + 1],
        &data[high],
        movements
    );

    return i + 1;
}

static void quick_sort_recursive(
    int data[],
    int low,
    int high,
    Count *comparisons,
    Count *movements,
    int depth,
    int *max_depth
)
{
    int pivot;

    if (depth > *max_depth) {
        *max_depth = depth;
    }

    if (low >= high) {
        return;
    }

    pivot = partition_lomuto(
        data,
        low,
        high,
        comparisons,
        movements
    );

    quick_sort_recursive(
        data,
        low,
        pivot - 1,
        comparisons,
        movements,
        depth + 1,
        max_depth
    );

    quick_sort_recursive(
        data,
        pivot + 1,
        high,
        comparisons,
        movements,
        depth + 1,
        max_depth
    );
}

static void sorted_array_insert(
    int data[],
    int *size,
    int value,
    Count *comparisons,
    Count *movements
)
{
    int pos = 0;
    int i;

    /* Insert after existing equal values. */
    while (pos < *size) {
        (*comparisons)++;
        if (data[pos] <= value) {
            pos++;
        }
        else {
            break;
        }
    }

    for (i = *size; i > pos; i--) {
        data[i] = data[i - 1];
        (*movements)++;
    }

    data[pos] = value;
    (*movements)++;
    (*size)++;
}

static void sorted_array_delete_one(
    int data[],
    int *size,
    int value,
    Count *comparisons,
    Count *movements
)
{
    int pos = -1;
    int i;

    for (i = 0; i < *size; i++) {
        (*comparisons)++;
        if (data[i] == value) {
            pos = i;
            break;
        }
    }

    if (pos < 0) {
        return;
    }

    for (i = pos; i < *size - 1; i++) {
        data[i] = data[i + 1];
        (*movements)++;
    }

    (*size)--;
}

void quick_array_process(
    int data[],
    int n,
    const Operation operations[],
    int operation_count,
    MethodStats *stats,
    int *output_count
)
{
    int size = n;
    int max_depth = 0;
    int i;

    stats->comparisons = 0;
    stats->movements = 0;
    stats->memory_bytes = 0;
    stats->final_count = 0;
    stats->max_depth = 0;

    if (n > 0) {
        quick_sort_recursive(
            data,
            0,
            n - 1,
            &stats->comparisons,
            &stats->movements,
            1,
            &max_depth
        );
    }

    for (i = 0; i < operation_count; i++) {
        if (operations[i].type == 0) {
            sorted_array_insert(
                data,
                &size,
                operations[i].value,
                &stats->comparisons,
                &stats->movements
            );
        }
        else {
            sorted_array_delete_one(
                data,
                &size,
                operations[i].value,
                &stats->comparisons,
                &stats->movements
            );
        }
    }

    stats->final_count = size;
    stats->max_depth = max_depth;

    /*
     * The sorted-array structure reserves enough storage for the maximum
     * possible count (all operations could be insertions). The original input
     * array is a common input cost and is not included.
     */
    stats->memory_bytes =
        sizeof(int) * (size_t)(n + operation_count) +
        sizeof(QuickFrameEstimate) * (size_t)max_depth;

    *output_count = size;
}

/* ========================= AVL TREE ========================= */

static int node_height(const AVLNode *node)
{
    return node == NULL ? 0 : node->height;
}

static int max_int(int a, int b)
{
    return a > b ? a : b;
}

static void update_height(AVLNode *node)
{
    if (node != NULL) {
        node->height =
            max_int(
                node_height(node->left),
                node_height(node->right)
            ) + 1;
    }
}

static int balance_factor(const AVLNode *node)
{
    if (node == NULL) {
        return 0;
    }

    return node_height(node->left) -
           node_height(node->right);
}

static AVLNode *rotate_right(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    update_height(y);
    update_height(x);

    return x;
}

static AVLNode *rotate_left(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    update_height(x);
    update_height(y);

    return y;
}

static AVLNode *rebalance(AVLNode *node)
{
    int bf;

    if (node == NULL) {
        return NULL;
    }

    update_height(node);
    bf = balance_factor(node);

    if (bf > 1) {
        if (balance_factor(node->left) < 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }

    if (bf < -1) {
        if (balance_factor(node->right) > 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }

    return node;
}

static int compare_key(
    int a,
    int b,
    Count *comparisons
)
{
    (*comparisons)++;

    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

static AVLNode *new_avl_node(
    int key,
    Count *movements
)
{
    AVLNode *node =
        (AVLNode *)malloc(sizeof(AVLNode));

    if (node == NULL) {
        return NULL;
    }

    node->key = key;
    (*movements)++;

    node->count = 1;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

static AVLNode *avl_insert_rec(
    AVLNode *node,
    int key,
    AVLTree *tree,
    int depth,
    int *ok
)
{
    int cmp;

    if (depth > tree->max_operation_depth) {
        tree->max_operation_depth = depth;
    }

    if (node == NULL) {
        AVLNode *created =
            new_avl_node(key, &tree->movements);

        if (created == NULL) {
            *ok = 0;
            return NULL;
        }

        tree->unique_count++;
        if (tree->unique_count > tree->peak_unique_count) {
            tree->peak_unique_count = tree->unique_count;
        }
        tree->total_count++;
        return created;
    }

    cmp = compare_key(
        key,
        node->key,
        &tree->comparisons
    );

    if (cmp == 0) {
        node->count++;
        tree->total_count++;
        return node;
    }

    if (cmp < 0) {
        AVLNode *child = avl_insert_rec(
            node->left,
            key,
            tree,
            depth + 1,
            ok
        );

        if (!*ok) {
            return node;
        }

        node->left = child;
    }
    else {
        AVLNode *child = avl_insert_rec(
            node->right,
            key,
            tree,
            depth + 1,
            ok
        );

        if (!*ok) {
            return node;
        }

        node->right = child;
    }

    return rebalance(node);
}

static AVLNode *find_min(AVLNode *node)
{
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }

    return node;
}

/* Remove the entire node with key, regardless of its duplicate count. */
static AVLNode *avl_remove_node_all_rec(
    AVLNode *node,
    int key,
    AVLTree *tree,
    int depth,
    int *removed_unique
)
{
    int cmp;

    if (depth > tree->max_operation_depth) {
        tree->max_operation_depth = depth;
    }

    if (node == NULL) {
        return NULL;
    }

    cmp = compare_key(
        key,
        node->key,
        &tree->comparisons
    );

    if (cmp < 0) {
        node->left = avl_remove_node_all_rec(
            node->left,
            key,
            tree,
            depth + 1,
            removed_unique
        );
    }
    else if (cmp > 0) {
        node->right = avl_remove_node_all_rec(
            node->right,
            key,
            tree,
            depth + 1,
            removed_unique
        );
    }
    else {
        if (node->left == NULL || node->right == NULL) {
            AVLNode *child =
                node->left != NULL
                ? node->left
                : node->right;

            free(node);
            tree->unique_count--;
            *removed_unique = 1;
            return child;
        }
        else {
            AVLNode *successor = find_min(node->right);
            int successor_key = successor->key;
            int successor_count = successor->count;
            int dummy_removed = 0;

            node->key = successor_key;
            tree->movements++;
            node->count = successor_count;

            node->right = avl_remove_node_all_rec(
                node->right,
                successor_key,
                tree,
                depth + 1,
                &dummy_removed
            );

            *removed_unique = 1;
        }
    }

    return rebalance(node);
}

static AVLNode *avl_delete_one_rec(
    AVLNode *node,
    int key,
    AVLTree *tree,
    int depth,
    int *deleted
)
{
    int cmp;

    if (depth > tree->max_operation_depth) {
        tree->max_operation_depth = depth;
    }

    if (node == NULL) {
        return NULL;
    }

    cmp = compare_key(
        key,
        node->key,
        &tree->comparisons
    );

    if (cmp < 0) {
        node->left = avl_delete_one_rec(
            node->left,
            key,
            tree,
            depth + 1,
            deleted
        );
    }
    else if (cmp > 0) {
        node->right = avl_delete_one_rec(
            node->right,
            key,
            tree,
            depth + 1,
            deleted
        );
    }
    else {
        *deleted = 1;
        tree->total_count--;

        if (node->count > 1) {
            node->count--;
            return node;
        }

        if (node->left == NULL || node->right == NULL) {
            AVLNode *child =
                node->left != NULL
                ? node->left
                : node->right;

            free(node);
            tree->unique_count--;
            return child;
        }
        else {
            AVLNode *successor = find_min(node->right);
            int successor_key = successor->key;
            int successor_count = successor->count;
            int removed_unique = 0;

            /*
             * Move the successor's complete key/count group here,
             * then remove that successor node entirely. Total element
             * count does not change further because those duplicates are moved.
             */
            node->key = successor_key;
            tree->movements++;
            node->count = successor_count;

            node->right = avl_remove_node_all_rec(
                node->right,
                successor_key,
                tree,
                depth + 1,
                &removed_unique
            );
        }
    }

    return rebalance(node);
}

int avl_process(
    const int original[],
    int n,
    const Operation operations[],
    int operation_count,
    MethodStats *stats,
    AVLTree *tree
)
{
    int i;

    tree->root = NULL;
    tree->total_count = 0;
    tree->unique_count = 0;
    tree->peak_unique_count = 0;
    tree->max_operation_depth = 0;
    tree->comparisons = 0;
    tree->movements = 0;

    for (i = 0; i < n; i++) {
        int ok = 1;

        tree->root = avl_insert_rec(
            tree->root,
            original[i],
            tree,
            1,
            &ok
        );

        if (!ok) {
            avl_destroy(tree);
            return 0;
        }
    }

    for (i = 0; i < operation_count; i++) {
        if (operations[i].type == 0) {
            int ok = 1;

            tree->root = avl_insert_rec(
                tree->root,
                operations[i].value,
                tree,
                1,
                &ok
            );

            if (!ok) {
                avl_destroy(tree);
                return 0;
            }
        }
        else {
            int deleted = 0;

            tree->root = avl_delete_one_rec(
                tree->root,
                operations[i].value,
                tree,
                1,
                &deleted
            );
        }
    }

    stats->comparisons = tree->comparisons;
    stats->movements = tree->movements;
    stats->final_count = tree->total_count;
    stats->max_depth = tree->max_operation_depth;
    stats->memory_bytes =
        sizeof(AVLTree) +
        sizeof(AVLNode) * (size_t)tree->peak_unique_count +
        sizeof(AVLFrameEstimate) *
            (size_t)tree->max_operation_depth;

    return 1;
}

static int avl_match_inorder(
    const AVLNode *node,
    const int array[],
    int count,
    int *index
)
{
    int i;

    if (node == NULL) {
        return 1;
    }

    if (!avl_match_inorder(
            node->left,
            array,
            count,
            index))
    {
        return 0;
    }

    for (i = 0; i < node->count; i++) {
        if (*index >= count ||
            array[*index] != node->key)
        {
            return 0;
        }
        (*index)++;
    }

    if (!avl_match_inorder(
            node->right,
            array,
            count,
            index))
    {
        return 0;
    }

    return 1;
}

int avl_matches_sorted_array(
    const AVLTree *tree,
    const int array[],
    int count
)
{
    int index = 0;

    if (tree == NULL || tree->total_count != count) {
        return 0;
    }

    if (!avl_match_inorder(
            tree->root,
            array,
            count,
            &index))
    {
        return 0;
    }

    return index == count;
}

static void avl_destroy_node(AVLNode *node)
{
    if (node == NULL) {
        return;
    }

    avl_destroy_node(node->left);
    avl_destroy_node(node->right);
    free(node);
}

void avl_destroy(AVLTree *tree)
{
    if (tree == NULL) {
        return;
    }

    avl_destroy_node(tree->root);
    tree->root = NULL;
    tree->total_count = 0;
    tree->unique_count = 0;
}
