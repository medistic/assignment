#include "sort.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define VALUE_MAX 1000000
#define SIZE_COUNT 6
#define TIME_REPEATS 5

typedef struct {
    uint64_t state;
} DataRNG;

typedef struct {
    MethodStats stats;
    double average_ms;
} Measurement;

static uint64_t splitmix64_next(DataRNG *rng)
{
    uint64_t z;

    rng->state += 0x9E3779B97F4A7C15ULL;
    z = rng->state;

    z = (z ^ (z >> 30)) *
        0xBF58476D1CE4E5B9ULL;

    z = (z ^ (z >> 27)) *
        0x94D049BB133111EBULL;

    return z ^ (z >> 31);
}

static int random_value(DataRNG *rng)
{
    return (int)(
        splitmix64_next(rng) %
        (uint64_t)(VALUE_MAX + 1)
    );
}

static double elapsed_ms(
    const struct timespec *start,
    const struct timespec *end
)
{
    double seconds =
        (double)(end->tv_sec - start->tv_sec);

    double nanoseconds =
        (double)(end->tv_nsec - start->tv_nsec);

    return seconds * 1000.0 +
           nanoseconds / 1000000.0;
}

static void generate_original(
    int data[],
    int n,
    DataRNG *rng
)
{
    int i;

    for (i = 0; i < n; i++) {
        data[i] = random_value(rng);
    }
}

static void generate_operations(
    Operation operations[],
    int count,
    DataRNG *rng
)
{
    int i;

    for (i = 0; i < count; i++) {
        int selector = random_value(rng);

        operations[i].type =
            (selector % 2 == 0) ? 0 : 1;

        /* The operation value is generated separately, as required. */
        operations[i].value = random_value(rng);
    }
}

static int verify_sorted(
    const int data[],
    int count
)
{
    int i;

    for (i = 1; i < count; i++) {
        if (data[i - 1] > data[i]) {
            return 0;
        }
    }

    return 1;
}

static int measure_both(
    const int original[],
    int n,
    const Operation operations[],
    int operation_count,
    Measurement *quick_result,
    Measurement *avl_result
)
{
    double quick_total_ms = 0.0;
    double avl_total_ms = 0.0;
    int repeat;

    int *quick_array = (int *)malloc(
        sizeof(int) * (size_t)(n + operation_count)
    );

    if (quick_array == NULL) {
        return 0;
    }

    for (repeat = 0; repeat < TIME_REPEATS; repeat++) {
        MethodStats quick_stats;
        MethodStats avl_stats;
        AVLTree tree;
        int quick_count = 0;
        struct timespec start;
        struct timespec end;

        if ((repeat % 2) == 0) {
            memcpy(
                quick_array,
                original,
                sizeof(int) * (size_t)n
            );

            timespec_get(&start, TIME_UTC);

            quick_array_process(
                quick_array,
                n,
                operations,
                operation_count,
                &quick_stats,
                &quick_count
            );

            timespec_get(&end, TIME_UTC);
            quick_total_ms += elapsed_ms(&start, &end);

            timespec_get(&start, TIME_UTC);

            if (!avl_process(
                    original,
                    n,
                    operations,
                    operation_count,
                    &avl_stats,
                    &tree))
            {
                free(quick_array);
                return 0;
            }

            timespec_get(&end, TIME_UTC);
            avl_total_ms += elapsed_ms(&start, &end);
        }
        else {
            timespec_get(&start, TIME_UTC);

            if (!avl_process(
                    original,
                    n,
                    operations,
                    operation_count,
                    &avl_stats,
                    &tree))
            {
                free(quick_array);
                return 0;
            }

            timespec_get(&end, TIME_UTC);
            avl_total_ms += elapsed_ms(&start, &end);

            memcpy(
                quick_array,
                original,
                sizeof(int) * (size_t)n
            );

            timespec_get(&start, TIME_UTC);

            quick_array_process(
                quick_array,
                n,
                operations,
                operation_count,
                &quick_stats,
                &quick_count
            );

            timespec_get(&end, TIME_UTC);
            quick_total_ms += elapsed_ms(&start, &end);
        }

        /* Validation is intentionally outside the timed regions. */
        if (!verify_sorted(quick_array, quick_count) ||
            !avl_matches_sorted_array(
                &tree,
                quick_array,
                quick_count))
        {
            avl_destroy(&tree);
            free(quick_array);
            return 0;
        }

        if (quick_stats.final_count != avl_stats.final_count) {
            avl_destroy(&tree);
            free(quick_array);
            return 0;
        }

        if (repeat == 0) {
            quick_result->stats = quick_stats;
            avl_result->stats = avl_stats;
        }
        else {
            if (quick_result->stats.comparisons !=
                    quick_stats.comparisons ||
                quick_result->stats.movements !=
                    quick_stats.movements ||
                quick_result->stats.final_count !=
                    quick_stats.final_count ||
                avl_result->stats.comparisons !=
                    avl_stats.comparisons ||
                avl_result->stats.movements !=
                    avl_stats.movements ||
                avl_result->stats.final_count !=
                    avl_stats.final_count)
            {
                avl_destroy(&tree);
                free(quick_array);
                return 0;
            }
        }

        avl_destroy(&tree);
    }

    quick_result->average_ms =
        quick_total_ms / TIME_REPEATS;

    avl_result->average_ms =
        avl_total_ms / TIME_REPEATS;

    free(quick_array);
    return 1;
}

int main(int argc, char *argv[])
{
    const int sizes[SIZE_COUNT] = {
        1000,
        2000,
        4000,
        8000,
        16000,
        20000
    };

    uint64_t seed;
    DataRNG rng;
    int index;

    if (argc >= 2) {
        seed = (uint64_t)strtoull(
            argv[1],
            NULL,
            10
        );
    }
    else {
        seed = (uint64_t)time(NULL);
    }

    rng.state = seed;

    printf(
        "Random seed : %llu\n",
        (unsigned long long)seed
    );

    printf(
        "Value range : 0 ~ %d\n",
        VALUE_MAX
    );

    printf(
        "Quick pivot : Median-of-Three\n"
    );

    printf(
        "Timing      : average of %d identical runs\n\n",
        TIME_REPEATS
    );

    for (index = 0; index < SIZE_COUNT; index++) {
        int n = sizes[index];
        int operation_count = n / 5;

        int *original = (int *)malloc(
            sizeof(int) * (size_t)n
        );

        Operation *operations =
            (Operation *)malloc(
                sizeof(Operation) *
                (size_t)operation_count
            );

        Measurement quick_result;
        Measurement avl_result;

        if (original == NULL || operations == NULL) {
            fprintf(
                stderr,
                "Error: memory allocation failed.\n"
            );

            free(original);
            free(operations);
            return 1;
        }

        generate_original(
            original,
            n,
            &rng
        );

        generate_operations(
            operations,
            operation_count,
            &rng
        );

        if (!measure_both(
                original,
                n,
                operations,
                operation_count,
                &quick_result,
                &avl_result))
        {
            fprintf(
                stderr,
                "Error: experiment failed for n=%d.\n",
                n
            );

            free(original);
            free(operations);
            return 1;
        }

        printf(
            "Data Size : %d\n",
            n
        );

        printf(
            "Operations: %d\n",
            operation_count
        );

        printf(
            "%-20s %18s %18s\n",
            "",
            "Quick Sort",
            "AVL Tree"
        );

        printf(
            "------------------------------------------------------------\n"
        );

        printf(
            "%-20s %15.6f ms %15.6f ms\n",
            "Total Time",
            quick_result.average_ms,
            avl_result.average_ms
        );

        printf(
            "%-20s %18lld %18lld\n",
            "Comparisons",
            quick_result.stats.comparisons,
            avl_result.stats.comparisons
        );

        printf(
            "%-20s %18lld %18lld\n",
            "Data Movements",
            quick_result.stats.movements,
            avl_result.stats.movements
        );

        printf(
            "%-20s %15zu B %15zu B\n",
            "Memory Usage",
            quick_result.stats.memory_bytes,
            avl_result.stats.memory_bytes
        );

        printf(
            "%-20s %18d %18d\n",
            "Final Data Count",
            quick_result.stats.final_count,
            avl_result.stats.final_count
        );

        printf(
            "%-20s %18d %18d\n",
            "Max Recursion Depth",
            quick_result.stats.max_depth,
            avl_result.stats.max_depth
        );

        printf(
            "Final Data Match     : Yes\n\n"
        );

        free(original);
        free(operations);
    }

    return 0;
}
