#include "bst.h"
#include "avl.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GENERATION_COUNT 100
#define SEARCH_COUNT 50
#define MIN_VALUE 0
#define MAX_VALUE 1000

typedef struct {
    int values[GENERATION_COUNT];
    int size;
    long long construction_comparisons;
} IntArray;

static void array_init(IntArray *array)
{
    array->size = 0;
    array->construction_comparisons = 0;
}

static int array_insert_unique(IntArray *array, int value)
{
    int i;
    for (i = 0; i < array->size; i++) {
        array->construction_comparisons++;
        if (array->values[i] == value) return 0;
    }
    array->values[array->size++] = value;
    return 1;
}

static int sequential_search(const IntArray *array, int key, int *comparisons)
{
    int i;
    *comparisons = 0;
    for (i = 0; i < array->size; i++) {
        (*comparisons)++;
        if (array->values[i] == key) return 1;
    }
    return 0;
}

static void print_values(const char *title, const int values[], int count)
{
    int i;
    printf("%s\n", title);
    for (i = 0; i < count; i++) {
        printf("%4d", values[i]);
        if ((i + 1) % 10 == 0 || i == count - 1) printf("\n");
        else printf(" ");
    }
}

int main(int argc, char *argv[])
{
    IntArray array;
    BST bst;
    AVLTree avl;
    int generated[GENERATION_COUNT];
    int search_keys[SEARCH_COUNT];
    unsigned int seed;
    int duplicate_count;
    int i;
    long long sequential_total = 0;
    long long bst_search_total = 0;
    long long avl_search_total = 0;
    int sequential_success = 0;
    int bst_success = 0;
    int avl_success = 0;

    if (argc >= 2) seed = (unsigned int)strtoul(argv[1], NULL, 10);
    else seed = (unsigned int)time(NULL);

    srand(seed);
    array_init(&array);
    bst_init(&bst);
    avl_init(&avl);

    for (i = 0; i < GENERATION_COUNT; i++) {
        int value = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
        generated[i] = value;
        array_insert_unique(&array, value);
        bst_insert(&bst, value);
        avl_insert(&avl, value);
    }

    duplicate_count = GENERATION_COUNT - array.size;

    for (i = 0; i < SEARCH_COUNT; i++) {
        search_keys[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
    }

    printf("Random seed: %u\n\n", seed);
    print_values("Generated 100 integers:", generated, GENERATION_COUNT);

    printf("\nStored distinct values : %d\n", array.size);
    printf("Duplicate values       : %d\n\n", duplicate_count);

    printf("Construction comparisons\n");
    printf("  Array : %lld\n", array.construction_comparisons);
    printf("  BST   : %lld\n", bst.construction_comparisons);
    printf("  AVL   : %lld\n", avl.construction_comparisons);

    printf("\nStructure\n");
    printf("  Array length : %d\n", array.size);
    printf("  BST height   : %d\n", bst_height(&bst));
    printf("  AVL height   : %d\n", avl_height(&avl));

    printf("\n");
    print_values("Generated 50 search keys:", search_keys, SEARCH_COUNT);

    printf("\n");
    printf("%-7s %-10s %-8s %-10s %-8s %-10s %-8s\n",
           "Key", "SeqResult", "SeqCmp", "BSTResult", "BSTCmp", "AVLResult", "AVLCmp");
    printf("-----------------------------------------------------------------------\n");

    for (i = 0; i < SEARCH_COUNT; i++) {
        int key = search_keys[i];
        int seq_cmp, bst_cmp, avl_cmp;
        int seq_found, bst_found, avl_found;

        seq_found = sequential_search(&array, key, &seq_cmp);
        bst_found = bst_search(&bst, key, &bst_cmp);
        avl_found = avl_search(&avl, key, &avl_cmp);

        sequential_total += seq_cmp;
        bst_search_total += bst_cmp;
        avl_search_total += avl_cmp;

        if (seq_found) sequential_success++;
        if (bst_found) bst_success++;
        if (avl_found) avl_success++;

        printf("%-7d %-10s %-8d %-10s %-8d %-10s %-8d\n",
               key,
               seq_found ? "Found" : "NotFound", seq_cmp,
               bst_found ? "Found" : "NotFound", bst_cmp,
               avl_found ? "Found" : "NotFound", avl_cmp);
    }

    printf("\nSearch summary\n");
    printf("Number of searches : %d\n\n", SEARCH_COUNT);

    printf("Sequential Search\n");
    printf("  Successful searches : %d\n", sequential_success);
    printf("  Total comparisons   : %lld\n", sequential_total);
    printf("  Average comparisons : %.2f\n\n", (double)sequential_total / SEARCH_COUNT);

    printf("BST Search\n");
    printf("  Successful searches : %d\n", bst_success);
    printf("  Total comparisons   : %lld\n", bst_search_total);
    printf("  Average comparisons : %.2f\n\n", (double)bst_search_total / SEARCH_COUNT);

    printf("AVL Search\n");
    printf("  Successful searches : %d\n", avl_success);
    printf("  Total comparisons   : %lld\n", avl_search_total);
    printf("  Average comparisons : %.2f\n", (double)avl_search_total / SEARCH_COUNT);

    if (array.size != bst.size || array.size != avl.size) {
        fprintf(stderr, "\nError: structure sizes do not match.\n");
        bst_destroy(&bst);
        avl_destroy(&avl);
        return 1;
    }

    bst_destroy(&bst);
    avl_destroy(&avl);
    return 0;
}
