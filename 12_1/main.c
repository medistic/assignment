#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define VALUE_MAX 1000000
#define SIZE_COUNT 6
#define TIMING_REPEATS 20

typedef long long Count;

typedef enum {
    PIVOT_FIRST = 0,
    PIVOT_RANDOM,
    PIVOT_MEDIAN3
} PivotMethod;

typedef struct {
    uint64_t state;
} RNG;

typedef struct {
    int low;
    int high;
} Range;

static uint64_t rng_next(RNG *rng)
{
    uint64_t x = rng->state;
    if (x == 0) x = 0x9E3779B97F4A7C15ULL;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    rng->state = x;
    return x * 2685821657736338717ULL;
}

static void rng_seed(RNG *rng, uint64_t seed)
{
    rng->state = seed ? seed : 0x9E3779B97F4A7C15ULL;
}

static void swap_int(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

static int median3_index(const int data[], int low, int high, Count *cmp)
{
    int mid = low + (high - low) / 2;
    int a = data[low], b = data[mid], c = data[high];

    (*cmp)++;
    if (a < b) {
        (*cmp)++;
        if (b < c) return mid;
        (*cmp)++;
        if (a < c) return high;
        return low;
    } else {
        (*cmp)++;
        if (a < c) return low;
        (*cmp)++;
        if (b < c) return high;
        return mid;
    }
}

static int choose_pivot(const int data[], int low, int high,
                        PivotMethod method, RNG *rng, Count *cmp)
{
    if (method == PIVOT_FIRST) return low;
    if (method == PIVOT_RANDOM) {
        uint64_t span = (uint64_t)(high - low + 1);
        return low + (int)(rng_next(rng) % span);
    }
    return median3_index(data, low, high, cmp);
}

static int partition_lomuto(int data[], int low, int high,
                            PivotMethod method, RNG *rng, Count *cmp)
{
    int pivot_index = choose_pivot(data, low, high, method, rng, cmp);
    int pivot_value;
    int i = low - 1;
    int j;

    swap_int(&data[pivot_index], &data[high]);
    pivot_value = data[high];

    for (j = low; j < high; j++) {
        (*cmp)++;
        if (data[j] <= pivot_value) {
            i++;
            if (i != j) swap_int(&data[i], &data[j]);
        }
    }

    swap_int(&data[i + 1], &data[high]);
    return i + 1;
}

static void quick_recursive_range(int data[], int low, int high,
                                  PivotMethod method, RNG *rng, Count *cmp)
{
    if (low < high) {
        int p = partition_lomuto(data, low, high, method, rng, cmp);
        quick_recursive_range(data, low, p - 1, method, rng, cmp);
        quick_recursive_range(data, p + 1, high, method, rng, cmp);
    }
}

static Count quick_recursive(int data[], int n, PivotMethod method, RNG *rng)
{
    Count cmp = 0;
    if (n > 1) quick_recursive_range(data, 0, n - 1, method, rng, &cmp);
    return cmp;
}

static Count quick_iterative(int data[], int n, PivotMethod method, RNG *rng)
{
    Count cmp = 0;
    Range *stack;
    int top = 0;

    if (n <= 1) return 0;

    stack = (Range *)malloc(sizeof(Range) * (size_t)n);
    if (!stack) return -1;

    stack[top++] = (Range){0, n - 1};

    while (top > 0) {
        Range r = stack[--top];
        if (r.low < r.high) {
            int p = partition_lomuto(data, r.low, r.high, method, rng, &cmp);
            if (r.low < p - 1) stack[top++] = (Range){r.low, p - 1};
            if (p + 1 < r.high) stack[top++] = (Range){p + 1, r.high};
        }
    }

    free(stack);
    return cmp;
}

static int is_sorted(const int data[], int n)
{
    int i;
    for (i = 1; i < n; i++) {
        if (data[i - 1] > data[i]) return 0;
    }
    return 1;
}

static double elapsed_ms(const struct timespec *s, const struct timespec *e)
{
    return (double)(e->tv_sec - s->tv_sec) * 1000.0 +
           (double)(e->tv_nsec - s->tv_nsec) / 1000000.0;
}

static int measure_bonus(const int original[], int n,
                         double *recursive_ms, double *iterative_ms)
{
    int *a = (int *)malloc(sizeof(int) * (size_t)n);
    int *b = (int *)malloc(sizeof(int) * (size_t)n);
    double rt = 0.0, it = 0.0;
    int r;

    if (!a || !b) {
        free(a); free(b);
        return 0;
    }

    for (r = 0; r < TIMING_REPEATS; r++) {
        struct timespec s, e;
        Count ca, cb;
        RNG ra, rb;
        rng_seed(&ra, 1);
        rng_seed(&rb, 1);

        if ((r % 2) == 0) {
            memcpy(a, original, sizeof(int) * (size_t)n);
            timespec_get(&s, TIME_UTC);
            ca = quick_recursive(a, n, PIVOT_MEDIAN3, &ra);
            timespec_get(&e, TIME_UTC);
            rt += elapsed_ms(&s, &e);

            memcpy(b, original, sizeof(int) * (size_t)n);
            timespec_get(&s, TIME_UTC);
            cb = quick_iterative(b, n, PIVOT_MEDIAN3, &rb);
            timespec_get(&e, TIME_UTC);
            it += elapsed_ms(&s, &e);
        } else {
            memcpy(b, original, sizeof(int) * (size_t)n);
            timespec_get(&s, TIME_UTC);
            cb = quick_iterative(b, n, PIVOT_MEDIAN3, &rb);
            timespec_get(&e, TIME_UTC);
            it += elapsed_ms(&s, &e);

            memcpy(a, original, sizeof(int) * (size_t)n);
            timespec_get(&s, TIME_UTC);
            ca = quick_recursive(a, n, PIVOT_MEDIAN3, &ra);
            timespec_get(&e, TIME_UTC);
            rt += elapsed_ms(&s, &e);
        }

        if (ca != cb || ca < 0 || cb < 0 || !is_sorted(a, n) || !is_sorted(b, n)) {
            free(a); free(b);
            return 0;
        }
    }

    *recursive_ms = rt / TIMING_REPEATS;
    *iterative_ms = it / TIMING_REPEATS;
    free(a); free(b);
    return 1;
}

int main(int argc, char *argv[])
{
    const int sizes[SIZE_COUNT] = {1000, 2000, 4000, 8000, 16000, 20000};
    uint64_t seed = (argc >= 2) ? (uint64_t)strtoull(argv[1], NULL, 10)
                                : (uint64_t)time(NULL);
    RNG data_rng;
    int si;

    rng_seed(&data_rng, seed);

    printf("Random seed : %llu\n", (unsigned long long)seed);
    printf("Value range : 0 ~ %d\n\n", VALUE_MAX);

    for (si = 0; si < SIZE_COUNT; si++) {
        int n = sizes[si];
        int *original = (int *)malloc(sizeof(int) * (size_t)n);
        int *first = (int *)malloc(sizeof(int) * (size_t)n);
        int *randomp = (int *)malloc(sizeof(int) * (size_t)n);
        int *median = (int *)malloc(sizeof(int) * (size_t)n);
        Count c_first, c_random, c_median;
        double rec_ms, iter_ms;
        RNG r1, r2, r3;
        int i;

        if (!original || !first || !randomp || !median) {
            fprintf(stderr, "Error: memory allocation failed.\n");
            free(original); free(first); free(randomp); free(median);
            return 1;
        }

        for (i = 0; i < n; i++) {
            original[i] = (int)(rng_next(&data_rng) % (VALUE_MAX + 1ULL));
        }

        memcpy(first, original, sizeof(int) * (size_t)n);
        memcpy(randomp, original, sizeof(int) * (size_t)n);
        memcpy(median, original, sizeof(int) * (size_t)n);

        rng_seed(&r1, seed ^ (uint64_t)n);
        rng_seed(&r2, seed ^ ((uint64_t)n << 32) ^ 0xA5A5A5A5A5A5A5A5ULL);
        rng_seed(&r3, seed ^ ((uint64_t)n << 16) ^ 0x5A5A5A5A5A5A5A5AULL);

        c_first = quick_recursive(first, n, PIVOT_FIRST, &r1);
        c_random = quick_recursive(randomp, n, PIVOT_RANDOM, &r2);
        c_median = quick_recursive(median, n, PIVOT_MEDIAN3, &r3);

        if (!is_sorted(first, n) || !is_sorted(randomp, n) || !is_sorted(median, n)) {
            fprintf(stderr, "Error: sorting verification failed for n=%d.\n", n);
            free(original); free(first); free(randomp); free(median);
            return 1;
        }

        if (!measure_bonus(original, n, &rec_ms, &iter_ms)) {
            fprintf(stderr, "Error: bonus timing failed for n=%d.\n", n);
            free(original); free(first); free(randomp); free(median);
            return 1;
        }

        printf("Data Size : %d\n", n);
        printf("Quick Sort (First)    : %lld comparisons\n", c_first);
        printf("Quick Sort (Random)   : %lld comparisons\n", c_random);
        printf("Quick Sort (Median-3) : %lld comparisons\n", c_median);
        printf("Sorted Verification   : Yes\n\n");
        printf("Bonus - Median-of-Three (%d runs average)\n", TIMING_REPEATS);
        printf("Recursive Quick Sort  : %.6f ms\n", rec_ms);
        printf("Iterative Quick Sort  : %.6f ms\n\n", iter_ms);

        free(original); free(first); free(randomp); free(median);
    }

    return 0;
}
