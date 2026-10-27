#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef DATA_COUNT
#define DATA_COUNT 10000
#endif

#ifndef RANDOM_TRIALS_DEFAULT
#define RANDOM_TRIALS_DEFAULT 100
#endif

#define KEY_MAX 1000000
#define PARTIAL_SWAPS 100

typedef struct {
    int key1;
    int key2;
    int original_index;
} Data;

typedef struct {
    unsigned long long comparisons;
    unsigned long long movements;
} SortStats;

typedef struct {
    unsigned long long total_comparisons;
    unsigned long long total_movements;
    int all_sorted;
} AggregateStats;

typedef void (*SortFunction)(Data *, int, SortStats *);

static void stats_reset(SortStats *s)
{
    s->comparisons = 0;
    s->movements = 0;
}

static void swap_data(Data *a, Data *b, SortStats *s)
{
    Data temp = *a;
    s->movements++;

    *a = *b;
    s->movements++;

    *b = temp;
    s->movements++;
}

static int random_key(void)
{
    /*
     * Two rand() calls are combined so that even implementations with
     * a relatively small RAND_MAX can cover the full 0..1,000,000 range.
     */
    unsigned long long base = (unsigned long long)RAND_MAX + 1ULL;
    unsigned long long value =
        (unsigned long long)(unsigned int)rand() * base +
        (unsigned long long)(unsigned int)rand();

    return (int)(value % (KEY_MAX + 1ULL));
}

static void generate_random_data(Data data[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        data[i].key1 = random_key();
        data[i].key2 = random_key();
        data[i].original_index = i;
    }
}

static int is_sorted_by_key1(const Data data[], int n)
{
    int i;

    for (i = 1; i < n; i++) {
        if (data[i - 1].key1 > data[i].key1) {
            return 0;
        }
    }

    return 1;
}

static int is_stable_result(const Data data[], int n)
{
    int i;

    for (i = 1; i < n; i++) {
        if (data[i - 1].key1 == data[i].key1 &&
            data[i - 1].original_index > data[i].original_index)
        {
            return 0;
        }
    }

    return 1;
}

void selection_sort(Data data[], int n, SortStats *stats)
{
    int i;
    int j;

    stats_reset(stats);

    for (i = 0; i < n - 1; i++) {
        int min_index = i;

        for (j = i + 1; j < n; j++) {
            stats->comparisons++;

            if (data[j].key1 < data[min_index].key1) {
                min_index = j;
            }
        }

        if (min_index != i) {
            swap_data(&data[i], &data[min_index], stats);
        }
    }
}

static void bubble_sort_internal(
    Data data[],
    int n,
    SortStats *stats,
    int early_termination
)
{
    int i;
    int j;

    stats_reset(stats);

    for (i = 0; i < n - 1; i++) {
        int swapped = 0;

        for (j = 0; j < n - 1 - i; j++) {
            stats->comparisons++;

            if (data[j].key1 > data[j + 1].key1) {
                swap_data(&data[j], &data[j + 1], stats);
                swapped = 1;
            }
        }

        if (early_termination && !swapped) {
            break;
        }
    }
}

void bubble_sort(Data data[], int n, SortStats *stats)
{
    bubble_sort_internal(data, n, stats, 0);
}

void bubble_sort_early(Data data[], int n, SortStats *stats)
{
    bubble_sort_internal(data, n, stats, 1);
}

static void insertion_sort_internal(
    Data data[],
    int n,
    SortStats *stats,
    int use_greater_equal
)
{
    int i;

    stats_reset(stats);

    for (i = 1; i < n; i++) {
        Data current = data[i];
        int j = i - 1;

        /* data[i] -> temporary variable */
        stats->movements++;

        while (j >= 0) {
            int should_move;

            stats->comparisons++;

            if (use_greater_equal) {
                should_move = data[j].key1 >= current.key1;
            }
            else {
                should_move = data[j].key1 > current.key1;
            }

            if (!should_move) {
                break;
            }

            data[j + 1] = data[j];
            stats->movements++;
            j--;
        }

        data[j + 1] = current;
        stats->movements++;
    }
}

void insertion_sort(Data data[], int n, SortStats *stats)
{
    insertion_sort_internal(data, n, stats, 0);
}

void insertion_sort_ge(Data data[], int n, SortStats *stats)
{
    insertion_sort_internal(data, n, stats, 1);
}

static void aggregate_add(
    AggregateStats *aggregate,
    const SortStats *stats,
    int sorted_ok
)
{
    aggregate->total_comparisons += stats->comparisons;
    aggregate->total_movements += stats->movements;

    if (!sorted_ok) {
        aggregate->all_sorted = 0;
    }
}

static void aggregate_init(AggregateStats *aggregate)
{
    aggregate->total_comparisons = 0;
    aggregate->total_movements = 0;
    aggregate->all_sorted = 1;
}

static void run_random_trials(
    int trials,
    AggregateStats *selection,
    AggregateStats *bubble,
    AggregateStats *bubble_early,
    AggregateStats *insertion
)
{
    Data *original = (Data *)malloc(sizeof(Data) * DATA_COUNT);
    Data *work = (Data *)malloc(sizeof(Data) * DATA_COUNT);
    int trial;

    if (original == NULL || work == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(original);
        free(work);
        exit(1);
    }

    aggregate_init(selection);
    aggregate_init(bubble);
    aggregate_init(bubble_early);
    aggregate_init(insertion);

    for (trial = 0; trial < trials; trial++) {
        SortStats stats;

        generate_random_data(original, DATA_COUNT);

        memcpy(work, original, sizeof(Data) * DATA_COUNT);
        selection_sort(work, DATA_COUNT, &stats);
        aggregate_add(selection, &stats, is_sorted_by_key1(work, DATA_COUNT));

        memcpy(work, original, sizeof(Data) * DATA_COUNT);
        bubble_sort(work, DATA_COUNT, &stats);
        aggregate_add(bubble, &stats, is_sorted_by_key1(work, DATA_COUNT));

        memcpy(work, original, sizeof(Data) * DATA_COUNT);
        bubble_sort_early(work, DATA_COUNT, &stats);
        aggregate_add(bubble_early, &stats, is_sorted_by_key1(work, DATA_COUNT));

        memcpy(work, original, sizeof(Data) * DATA_COUNT);
        insertion_sort(work, DATA_COUNT, &stats);
        aggregate_add(insertion, &stats, is_sorted_by_key1(work, DATA_COUNT));
    }

    free(original);
    free(work);
}

static void prepare_sorted_data(const Data source[], Data dest[], int n)
{
    SortStats ignored_stats;

    memcpy(dest, source, sizeof(Data) * n);

    /*
     * The program's own insertion sort is used only to prepare the
     * experimental sorted input.  These preparation operations are
     * deliberately excluded from measured statistics.
     */
    insertion_sort(dest, n, &ignored_stats);
}

static void prepare_reverse_data(const Data sorted[], Data reverse[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        reverse[i] = sorted[n - 1 - i];
    }
}

static void prepare_partially_sorted_data(
    const Data sorted[],
    Data partial[],
    int n
)
{
    int i;

    memcpy(partial, sorted, sizeof(Data) * n);

    /*
     * AI-assisted design choice:
     * start from fully sorted data and perform 100 random pair swaps.
     * For n=10,000, this is 1% as many swaps as the number of records.
     * Preparation cost is excluded from sort statistics.
     */
    for (i = 0; i < PARTIAL_SWAPS; i++) {
        int a = rand() % n;
        int b = rand() % n;
        Data temp;

        if (a == b) {
            i--;
            continue;
        }

        temp = partial[a];
        partial[a] = partial[b];
        partial[b] = temp;
    }
}

static void print_state_result(
    const char *algorithm,
    const SortStats *stats,
    int sorted_ok
)
{
    printf("%-34s Comparisons : %llu\n", algorithm, stats->comparisons);
    printf("%-34s Movements   : %llu\n", "", stats->movements);
    printf("%-34s Sorted      : %s\n", "", sorted_ok ? "Yes" : "No");
}

static void run_input_state_experiment(
    const char *title,
    const Data input[]
)
{
    Data *work = (Data *)malloc(sizeof(Data) * DATA_COUNT);
    SortStats stats;

    if (work == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    printf("------------------------------------\n");
    printf("%s\n\n", title);

    memcpy(work, input, sizeof(Data) * DATA_COUNT);
    selection_sort(work, DATA_COUNT, &stats);
    print_state_result(
        "Selection Sort",
        &stats,
        is_sorted_by_key1(work, DATA_COUNT)
    );
    printf("\n");

    memcpy(work, input, sizeof(Data) * DATA_COUNT);
    bubble_sort(work, DATA_COUNT, &stats);
    print_state_result(
        "Bubble Sort",
        &stats,
        is_sorted_by_key1(work, DATA_COUNT)
    );
    printf("\n");

    memcpy(work, input, sizeof(Data) * DATA_COUNT);
    bubble_sort_early(work, DATA_COUNT, &stats);
    print_state_result(
        "Bubble Sort (Early Termination)",
        &stats,
        is_sorted_by_key1(work, DATA_COUNT)
    );
    printf("\n");

    memcpy(work, input, sizeof(Data) * DATA_COUNT);
    insertion_sort(work, DATA_COUNT, &stats);
    print_state_result(
        "Insertion Sort (>)",
        &stats,
        is_sorted_by_key1(work, DATA_COUNT)
    );
    printf("\n");

    free(work);
}

static void print_aggregate(
    const char *name,
    const AggregateStats *stats,
    int trials
)
{
    printf("%s\n", name);
    printf("  Total Comparisons   : %llu\n", stats->total_comparisons);
    printf(
        "  Average Comparisons : %.2f\n",
        (double)stats->total_comparisons / trials
    );
    printf("  Total Movements     : %llu\n", stats->total_movements);
    printf(
        "  Average Movements   : %.2f\n",
        (double)stats->total_movements / trials
    );
    printf("  All Results Sorted  : %s\n\n", stats->all_sorted ? "Yes" : "No");
}

static void print_small_order(const Data data[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }

        printf(
            "(%d,%d,idx=%d)",
            data[i].key1,
            data[i].key2,
            data[i].original_index
        );
    }

    printf("\n");
}

static void stability_verification(void)
{
    Data test[] = {
        {5, 100, 0},
        {3, 200, 1},
        {5, 300, 2},
        {2, 400, 3},
        {3, 500, 4},
        {5, 600, 5},
        {2, 700, 6},
        {3, 800, 7}
    };

    const int n = (int)(sizeof(test) / sizeof(test[0]));
    Data work[8];
    SortStats stats;

    printf("------------------------------------\n");
    printf("Stability Verification\n\n");

    printf("Test data (Key1, Key2, OriginalIndex):\n");
    printf("(5,100,0) (3,200,1) (5,300,2) (2,400,3)\n");
    printf("(3,500,4) (5,600,5) (2,700,6) (3,800,7)\n\n");

    memcpy(work, test, sizeof(test));
    selection_sort(work, n, &stats);
    printf(
        "Selection Sort       : %s\n",
        is_stable_result(work, n) ? "Stable" : "Unstable"
    );
    printf("  Order: ");
    print_small_order(work, n);

    memcpy(work, test, sizeof(test));
    bubble_sort_early(work, n, &stats);
    printf(
        "Bubble Sort          : %s\n",
        is_stable_result(work, n) ? "Stable" : "Unstable"
    );
    printf("  Order: ");
    print_small_order(work, n);

    memcpy(work, test, sizeof(test));
    insertion_sort(work, n, &stats);
    printf(
        "Insertion Sort (>)   : %s\n",
        is_stable_result(work, n) ? "Stable" : "Unstable"
    );
    printf("  Order: ");
    print_small_order(work, n);

    memcpy(work, test, sizeof(test));
    insertion_sort_ge(work, n, &stats);
    printf(
        "Insertion Sort (>=)  : %s\n",
        is_stable_result(work, n) ? "Stable" : "Unstable"
    );
    printf("  Order: ");
    print_small_order(work, n);
}

int main(int argc, char *argv[])
{
    unsigned int seed;
    int trials = RANDOM_TRIALS_DEFAULT;

    AggregateStats selection_random;
    AggregateStats bubble_random;
    AggregateStats bubble_early_random;
    AggregateStats insertion_random;

    Data *base = NULL;
    Data *sorted = NULL;
    Data *reverse = NULL;
    Data *partial = NULL;

    if (argc >= 2) {
        seed = (unsigned int)strtoul(argv[1], NULL, 10);
    }
    else {
        seed = (unsigned int)time(NULL);
    }

    if (argc >= 3) {
        trials = atoi(argv[2]);

        if (trials <= 0) {
            fprintf(stderr, "Trials must be a positive integer.\n");
            return 1;
        }
    }

    srand(seed);

    printf("Data Count : %d\n", DATA_COUNT);
    printf("Random Seed: %u\n\n", seed);

    printf("Random Data\n");
    printf("Number of Trials : %d\n\n", trials);

    run_random_trials(
        trials,
        &selection_random,
        &bubble_random,
        &bubble_early_random,
        &insertion_random
    );

    print_aggregate("Selection Sort", &selection_random, trials);
    print_aggregate("Bubble Sort", &bubble_random, trials);
    print_aggregate(
        "Bubble Sort (Early Termination)",
        &bubble_early_random,
        trials
    );
    print_aggregate("Insertion Sort (>)", &insertion_random, trials);

    base = (Data *)malloc(sizeof(Data) * DATA_COUNT);
    sorted = (Data *)malloc(sizeof(Data) * DATA_COUNT);
    reverse = (Data *)malloc(sizeof(Data) * DATA_COUNT);
    partial = (Data *)malloc(sizeof(Data) * DATA_COUNT);

    if (base == NULL || sorted == NULL || reverse == NULL || partial == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(base);
        free(sorted);
        free(reverse);
        free(partial);
        return 1;
    }

    /* One common source is used to prepare all non-random input states. */
    generate_random_data(base, DATA_COUNT);
    prepare_sorted_data(base, sorted, DATA_COUNT);
    prepare_reverse_data(sorted, reverse, DATA_COUNT);
    prepare_partially_sorted_data(sorted, partial, DATA_COUNT);

    run_input_state_experiment("Sorted Data", sorted);
    run_input_state_experiment("Reverse-Sorted Data", reverse);
    run_input_state_experiment("Partially Sorted Data", partial);

    stability_verification();

    printf("\nPartial sorting method:\n");
    printf(
        "Start with fully sorted data and perform %d random pair swaps.\n",
        PARTIAL_SWAPS
    );

    free(base);
    free(sorted);
    free(reverse);
    free(partial);

    return 0;
}
