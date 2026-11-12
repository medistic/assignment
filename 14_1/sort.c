#include "sort.h"

#include <stdlib.h>

typedef struct {
    int heap_size;
    int root;
    int largest;
    int left;
    int right;
    int temp;
} HeapAuxEstimate;

typedef struct {
    int left;
    int right;
    int middle;
    int *data;
    int *temp;
    Count *comparisons;
} MergeFrameEstimate;

typedef struct {
    int low;
    int high;
    int pivot;
    int *data;
    PivotRNG *rng;
    Count *comparisons;
} QuickFrameEstimate;

static void swap_int(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ========================= HEAP SORT ========================= */

static void heapify_down(
    int data[],
    int heap_size,
    int root,
    Count *comparisons
)
{
    for (;;) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < heap_size) {
            (*comparisons)++;
            if (data[left] > data[largest]) {
                largest = left;
            }
        }

        if (right < heap_size) {
            (*comparisons)++;
            if (data[right] > data[largest]) {
                largest = right;
            }
        }

        if (largest == root) {
            break;
        }

        swap_int(&data[root], &data[largest]);
        root = largest;
    }
}

SortStats heap_sort(int data[], int n)
{
    SortStats stats;
    int i;

    stats.comparisons = 0;
    stats.max_depth = 1;
    stats.memory_bytes = sizeof(HeapAuxEstimate);

    if (data == NULL || n <= 1) {
        return stats;
    }

    for (i = n / 2 - 1; i >= 0; i--) {
        heapify_down(data, n, i, &stats.comparisons);
    }

    for (i = n - 1; i > 0; i--) {
        swap_int(&data[0], &data[i]);
        heapify_down(data, i, 0, &stats.comparisons);
    }

    return stats;
}

/* ========================= MERGE SORT ========================= */

static void merge_ranges(
    int data[],
    int temp[],
    int left,
    int middle,
    int right,
    Count *comparisons
)
{
    int i = left;
    int j = middle + 1;
    int k = left;

    while (i <= middle && j <= right) {
        (*comparisons)++;

        if (data[i] <= data[j]) {
            temp[k++] = data[i++];
        }
        else {
            temp[k++] = data[j++];
        }
    }

    while (i <= middle) {
        temp[k++] = data[i++];
    }

    while (j <= right) {
        temp[k++] = data[j++];
    }

    for (i = left; i <= right; i++) {
        data[i] = temp[i];
    }
}

static void merge_sort_recursive(
    int data[],
    int temp[],
    int left,
    int right,
    Count *comparisons,
    int depth,
    int *max_depth
)
{
    int middle;

    if (depth > *max_depth) {
        *max_depth = depth;
    }

    if (left >= right) {
        return;
    }

    middle = left + (right - left) / 2;

    merge_sort_recursive(
        data, temp, left, middle,
        comparisons, depth + 1, max_depth
    );

    merge_sort_recursive(
        data, temp, middle + 1, right,
        comparisons, depth + 1, max_depth
    );

    merge_ranges(
        data, temp, left, middle, right, comparisons
    );
}

SortStats merge_sort(int data[], int n)
{
    SortStats stats;
    int *temp;

    stats.comparisons = 0;
    stats.max_depth = 0;
    stats.memory_bytes = 0;

    if (data == NULL || n <= 0) {
        return stats;
    }

    temp = (int *)malloc(sizeof(int) * (size_t)n);

    if (temp == NULL) {
        stats.comparisons = -1;
        return stats;
    }

    merge_sort_recursive(
        data, temp, 0, n - 1,
        &stats.comparisons, 1, &stats.max_depth
    );

    stats.memory_bytes =
        sizeof(int) * (size_t)n +
        sizeof(MergeFrameEstimate) * (size_t)stats.max_depth;

    free(temp);
    return stats;
}

/* ========================= QUICK SORT ========================= */

static uint64_t pivot_rng_next(PivotRNG *rng)
{
    uint64_t x = rng->state;

    if (x == 0) {
        x = 0x9E3779B97F4A7C15ULL;
    }

    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;

    rng->state = x;
    return x * 2685821657736338717ULL;
}

void pivot_rng_seed(PivotRNG *rng, uint64_t seed)
{
    if (rng == NULL) {
        return;
    }

    rng->state = seed != 0
        ? seed
        : 0x9E3779B97F4A7C15ULL;
}

static int median3_index(
    const int data[],
    int low,
    int high,
    Count *comparisons
)
{
    int middle = low + (high - low) / 2;
    int a = data[low];
    int b = data[middle];
    int c = data[high];

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

static int choose_pivot_index(
    const int data[],
    int low,
    int high,
    PivotMethod method,
    PivotRNG *rng,
    Count *comparisons
)
{
    if (method == PIVOT_FIRST) {
        return low;
    }

    if (method == PIVOT_RANDOM) {
        uint64_t span = (uint64_t)(high - low + 1);
        return low + (int)(pivot_rng_next(rng) % span);
    }

    return median3_index(data, low, high, comparisons);
}

static int partition_lomuto(
    int data[],
    int low,
    int high,
    PivotMethod method,
    PivotRNG *rng,
    Count *comparisons
)
{
    int pivot_index = choose_pivot_index(
        data, low, high, method, rng, comparisons
    );
    int pivot_value;
    int i;
    int j;

    swap_int(&data[pivot_index], &data[high]);
    pivot_value = data[high];
    i = low - 1;

    for (j = low; j < high; j++) {
        (*comparisons)++;

        if (data[j] <= pivot_value) {
            i++;
            if (i != j) {
                swap_int(&data[i], &data[j]);
            }
        }
    }

    swap_int(&data[i + 1], &data[high]);
    return i + 1;
}

static void quick_sort_recursive(
    int data[],
    int low,
    int high,
    PivotMethod method,
    PivotRNG *rng,
    Count *comparisons,
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
        data, low, high, method, rng, comparisons
    );

    quick_sort_recursive(
        data, low, pivot - 1, method, rng,
        comparisons, depth + 1, max_depth
    );

    quick_sort_recursive(
        data, pivot + 1, high, method, rng,
        comparisons, depth + 1, max_depth
    );
}

SortStats quick_sort(
    int data[],
    int n,
    PivotMethod method,
    PivotRNG *rng
)
{
    SortStats stats;

    stats.comparisons = 0;
    stats.max_depth = 0;
    stats.memory_bytes = 0;

    if (data == NULL || n <= 0) {
        return stats;
    }

    quick_sort_recursive(
        data, 0, n - 1, method, rng,
        &stats.comparisons, 1, &stats.max_depth
    );

    stats.memory_bytes =
        sizeof(QuickFrameEstimate) * (size_t)stats.max_depth;

    return stats;
}

int is_sorted(const int data[], int n)
{
    int i;

    for (i = 1; i < n; i++) {
        if (data[i - 1] > data[i]) {
            return 0;
        }
    }

    return 1;
}
