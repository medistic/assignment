#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_COUNT 100
#define SEARCH_COUNT 50
#define MIN_VALUE 0
#define MAX_VALUE 1000
#define VALUE_RANGE (MAX_VALUE - MIN_VALUE + 1)

static int sequential_search(const int array[], int size, int key, int *comparisons)
{
    int i;
    *comparisons = 0;

    for (i = 0; i < size; i++) {
        (*comparisons)++;
        if (array[i] == key) return 1;
    }
    return 0;
}

static void generate_unique_data(int array[], int count)
{
    int used[VALUE_RANGE] = {0};
    int generated = 0;

    while (generated < count) {
        int value = MIN_VALUE + rand() % VALUE_RANGE;
        if (!used[value - MIN_VALUE]) {
            used[value - MIN_VALUE] = 1;
            array[generated++] = value;
        }
    }
}

static void generate_search_keys(int keys[], int count)
{
    int i;
    for (i = 0; i < count; i++)
        keys[i] = MIN_VALUE + rand() % VALUE_RANGE;
}

static void print_int_list(const char *title, const int values[], int count)
{
    int i;
    printf("%s\n", title);

    for (i = 0; i < count; i++) {
        printf("%4d", values[i]);
        if ((i + 1) % 10 == 0 || i == count - 1)
            printf("\n");
        else
            printf(" ");
    }
}

int main(int argc, char *argv[])
{
    int data[DATA_COUNT];
    int search_keys[SEARCH_COUNT];
    BST tree;
    unsigned int seed;
    long long sequential_total = 0;
    long long bst_total = 0;
    int sequential_success = 0;
    int bst_success = 0;
    int i;

    if (argc >= 2)
        seed = (unsigned int)strtoul(argv[1], NULL, 10);
    else
        seed = (unsigned int)time(NULL);

    srand(seed);
    bst_init(&tree);

    generate_unique_data(data, DATA_COUNT);

    for (i = 0; i < DATA_COUNT; i++) {
        if (!bst_insert(&tree, data[i])) {
            fprintf(stderr, "Error: failed to insert %d into BST.\n", data[i]);
            bst_destroy(&tree);
            return 1;
        }
    }

    generate_search_keys(search_keys, SEARCH_COUNT);

    printf("Random seed: %u\n\n", seed);
    print_int_list("Generated 100 unique integers:", data, DATA_COUNT);
    printf("\nBST build comparisons: %lld\n\n", tree.build_comparisons);
    print_int_list("Generated 50 search keys:", search_keys, SEARCH_COUNT);

    printf("\n%-10s %-12s %-18s %-12s %-18s\n",
           "Key", "Seq Result", "Seq Comparisons", "BST Result", "BST Comparisons");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < SEARCH_COUNT; i++) {
        int seq_comparisons;
        int bst_comparisons;
        int seq_found;
        int bst_found;
        int key = search_keys[i];

        seq_found = sequential_search(data, DATA_COUNT, key, &seq_comparisons);
        bst_found = bst_search(&tree, key, &bst_comparisons);

        sequential_total += seq_comparisons;
        bst_total += bst_comparisons;

        if (seq_found) sequential_success++;
        if (bst_found) bst_success++;

        printf("%-10d %-12s %-18d %-12s %-18d\n",
               key,
               seq_found ? "Found" : "Not Found",
               seq_comparisons,
               bst_found ? "Found" : "Not Found",
               bst_comparisons);
    }

    printf("\nNumber of searches: %d\n\n", SEARCH_COUNT);

    printf("Sequential Search\n");
    printf("  Total comparisons   : %lld\n", sequential_total);
    printf("  Average comparisons : %.2f\n", (double)sequential_total / SEARCH_COUNT);
    printf("  Successful searches : %d\n\n", sequential_success);

    printf("BST Search\n");
    printf("  Total comparisons   : %lld\n", bst_total);
    printf("  Average comparisons : %.2f\n", (double)bst_total / SEARCH_COUNT);
    printf("  Successful searches : %d\n\n", bst_success);

    printf("BST Construction\n");
    printf("  Build comparisons   : %lld\n\n", tree.build_comparisons);

    printf("Search-only comparison\n");
    printf("  Sequential total    : %lld\n", sequential_total);
    printf("  BST total           : %lld\n\n", bst_total);

    printf("Including BST construction cost\n");
    printf("  Sequential total    : %lld\n", sequential_total);
    printf("  BST build + search  : %lld\n", tree.build_comparisons + bst_total);

    if (sequential_total > bst_total) {
        printf("\nBST saved %lld comparisons during the 50 searches.\n",
               sequential_total - bst_total);
    }

    if (sequential_total > tree.build_comparisons + bst_total) {
        printf("Even after construction cost, BST used %lld fewer comparisons.\n",
               sequential_total - (tree.build_comparisons + bst_total));
    }
    else {
        printf("For this run, BST construction cost has not yet been recovered.\n");
    }

    bst_destroy(&tree);
    return 0;
}
