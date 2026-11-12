#include "sort.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VALUE_MAX 1000000
#define SIZE_COUNT 6
#define TIME_REPEATS 50

typedef struct {
    uint64_t state;
} DataRNG;

typedef struct {
    SortStats stats;
    double average_ms;
} Measurement;

static uint64_t splitmix64_next(DataRNG *rng)
{
    uint64_t z;

    rng->state += 0x9E3779B97F4A7C15ULL;
    z = rng->state;

    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;

    return z ^ (z >> 31);
}

static int random_value(DataRNG *rng)
{
    return (int)(
        splitmix64_next(rng) % (uint64_t)(VALUE_MAX + 1)
    );
}

static void generate_random_data(int data[], int n, DataRNG *rng)
{
    int i;

    for (i = 0; i < n; i++) {
        data[i] = random_value(rng);
    }
}

static void copy_data(int destination[], const int source[], int n)
{
    memcpy(destination, source, sizeof(int) * (size_t)n);
}

static double elapsed_ms(
    const struct timespec *start,
    const struct timespec *end
)
{
    double seconds = (double)(end->tv_sec - start->tv_sec);
    double nanoseconds = (double)(end->tv_nsec - start->tv_nsec);

    return seconds * 1000.0 + nanoseconds / 1000000.0;
}

static uint64_t pivot_seed(
    uint64_t base_seed,
    int n,
    PivotMethod method
)
{
    return base_seed ^
           ((uint64_t)n << 32) ^
           ((uint64_t)method * 0x9E3779B97F4A7C15ULL);
}

static int measure_heap(
    const int original[],
    int n,
    Measurement *measurement
)
{
    int *work;
    double total_ms = 0.0;
    int repeat;

    work = (int *)malloc(sizeof(int) * (size_t)n);
    if (work == NULL) {
        return 0;
    }

    for (repeat = 0; repeat < TIME_REPEATS; repeat++) {
        struct timespec start;
        struct timespec end;
        SortStats stats;

        copy_data(work, original, n);

        timespec_get(&start, TIME_UTC);
        stats = heap_sort(work, n);
        timespec_get(&end, TIME_UTC);

        if (!is_sorted(work, n)) {
            free(work);
            return 0;
        }

        total_ms += elapsed_ms(&start, &end);

        if (repeat == 0) {
            measurement->stats = stats;
        }
        else if (
            stats.comparisons != measurement->stats.comparisons ||
            stats.memory_bytes != measurement->stats.memory_bytes
        ) {
            free(work);
            return 0;
        }
    }

    measurement->average_ms = total_ms / TIME_REPEATS;
    free(work);
    return 1;
}

static int measure_merge(
    const int original[],
    int n,
    Measurement *measurement
)
{
    int *work;
    double total_ms = 0.0;
    int repeat;

    work = (int *)malloc(sizeof(int) * (size_t)n);
    if (work == NULL) {
        return 0;
    }

    for (repeat = 0; repeat < TIME_REPEATS; repeat++) {
        struct timespec start;
        struct timespec end;
        SortStats stats;

        copy_data(work, original, n);

        timespec_get(&start, TIME_UTC);
        stats = merge_sort(work, n);
        timespec_get(&end, TIME_UTC);

        if (stats.comparisons < 0 || !is_sorted(work, n)) {
            free(work);
            return 0;
        }

        total_ms += elapsed_ms(&start, &end);

        if (repeat == 0) {
            measurement->stats = stats;
        }
        else if (
            stats.comparisons != measurement->stats.comparisons ||
            stats.memory_bytes != measurement->stats.memory_bytes
        ) {
            free(work);
            return 0;
        }
    }

    measurement->average_ms = total_ms / TIME_REPEATS;
    free(work);
    return 1;
}

static int measure_quick(
    const int original[],
    int n,
    uint64_t seed,
    PivotMethod method,
    Measurement *measurement
)
{
    int *work;
    double total_ms = 0.0;
    int repeat;

    work = (int *)malloc(sizeof(int) * (size_t)n);
    if (work == NULL) {
        return 0;
    }

    for (repeat = 0; repeat < TIME_REPEATS; repeat++) {
        struct timespec start;
        struct timespec end;
        SortStats stats;
        PivotRNG rng;

        copy_data(work, original, n);

        pivot_rng_seed(&rng, pivot_seed(seed, n, method));

        timespec_get(&start, TIME_UTC);
        stats = quick_sort(work, n, method, &rng);
        timespec_get(&end, TIME_UTC);

        if (!is_sorted(work, n)) {
            free(work);
            return 0;
        }

        total_ms += elapsed_ms(&start, &end);

        if (repeat == 0) {
            measurement->stats = stats;
        }
        else if (
            stats.comparisons != measurement->stats.comparisons ||
            stats.memory_bytes != measurement->stats.memory_bytes
        ) {
            free(work);
            return 0;
        }
    }

    measurement->average_ms = total_ms / TIME_REPEATS;
    free(work);
    return 1;
}

static void print_row(const char *name, const Measurement *m)
{
    printf(
        "%-18s %15lld %12.6f %15zu %10d\n",
        name,
        m->stats.comparisons,
        m->average_ms,
        m->stats.memory_bytes,
        m->stats.max_depth
    );
}

int main(int argc, char *argv[])
{
    const int sizes[SIZE_COUNT] = {
        1000, 2000, 4000, 8000, 16000, 20000
    };

    uint64_t seed;
    DataRNG data_rng;
    int index;

    if (argc >= 2) {
        seed = (uint64_t)strtoull(argv[1], NULL, 10);
    }
    else {
        seed = (uint64_t)time(NULL);
    }

    data_rng.state = seed;

    printf("Random seed : %llu\n", (unsigned long long)seed);
    printf("Value range : 0 ~ %d\n", VALUE_MAX);
    printf("Timing      : average of %d runs\n\n", TIME_REPEATS);

    for (index = 0; index < SIZE_COUNT; index++) {
        int n = sizes[index];
        int *original;

        Measurement heap_result = {{0, 0, 0}, 0.0};
        Measurement merge_result = {{0, 0, 0}, 0.0};
        Measurement first_result = {{0, 0, 0}, 0.0};
        Measurement random_result = {{0, 0, 0}, 0.0};
        Measurement median_result = {{0, 0, 0}, 0.0};

        original = (int *)malloc(sizeof(int) * (size_t)n);
        if (original == NULL) {
            fprintf(stderr, "Error: memory allocation failed.\n");
            return 1;
        }

        generate_random_data(original, n, &data_rng);

        if (!measure_heap(original, n, &heap_result) ||
            !measure_merge(original, n, &merge_result) ||
            !measure_quick(original, n, seed, PIVOT_FIRST, &first_result) ||
            !measure_quick(original, n, seed, PIVOT_RANDOM, &random_result) ||
            !measure_quick(original, n, seed, PIVOT_MEDIAN3, &median_result))
        {
            fprintf(stderr, "Error: measurement failed for n=%d.\n", n);
            free(original);
            return 1;
        }

        printf("Data Size : %d\n", n);
        printf(
            "%-18s %15s %12s %15s %10s\n",
            "Algorithm", "Comparisons", "Time(ms)", "Memory(Bytes)", "MaxDepth"
        );
        printf("--------------------------------------------------------------------------\n");

        print_row("Heap Sort", &heap_result);
        print_row("Merge Sort", &merge_result);
        print_row("Quick (First)", &first_result);
        print_row("Quick (Random)", &random_result);
        print_row("Quick (Median3)", &median_result);

        printf("Sorted Verification : Yes\n\n");
        free(original);
    }

    return 0;
}
