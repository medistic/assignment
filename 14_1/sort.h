#ifndef SORT_H
#define SORT_H

#include <stdint.h>
#include <stddef.h>

typedef long long Count;

typedef enum {
    PIVOT_FIRST = 0,
    PIVOT_RANDOM,
    PIVOT_MEDIAN3
} PivotMethod;

typedef struct {
    uint64_t state;
} PivotRNG;

typedef struct {
    Count comparisons;
    int max_depth;
    size_t memory_bytes;
} SortStats;

void pivot_rng_seed(PivotRNG *rng, uint64_t seed);

SortStats heap_sort(int data[], int n);
SortStats merge_sort(int data[], int n);
SortStats quick_sort(int data[], int n, PivotMethod method, PivotRNG *rng);

int is_sorted(const int data[], int n);

#endif
