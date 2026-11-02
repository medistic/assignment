#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VALUE_MAX 1000000
#define SIZE_COUNT 6

typedef long long Count;

static unsigned int next_random_u32(void) {
    unsigned int a = (unsigned int)rand();
    unsigned int b = (unsigned int)rand();
    return (a << 16) ^ b;
}

static int random_value(void) {
    return (int)(next_random_u32() % (VALUE_MAX + 1U));
}

static void fill_random_data(int data[], int n) {
    int i;
    for (i = 0; i < n; i++) data[i] = random_value();
}

static Count insertion_sort(int data[], int n) {
    Count comparisons = 0;
    int i;
    for (i = 1; i < n; i++) {
        int current = data[i];
        int j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (data[j] > current) {
                data[j + 1] = data[j];
                j--;
            } else {
                break;
            }
        }
        data[j + 1] = current;
    }
    return comparisons;
}

static Count shell_sort_original(int data[], int n) {
    Count comparisons = 0;
    int gap;
    for (gap = n / 2; gap > 0; gap /= 2) {
        int i;
        for (i = gap; i < n; i++) {
            int current = data[i];
            int j = i;
            while (j >= gap) {
                comparisons++;
                if (data[j - gap] > current) {
                    data[j] = data[j - gap];
                    j -= gap;
                } else {
                    break;
                }
            }
            data[j] = current;
        }
    }
    return comparisons;
}

static Count shell_sort_knuth(int data[], int n) {
    Count comparisons = 0;
    int gap = 1;
    int i;
    while (gap < n / 3) gap = 3 * gap + 1;
    while (gap >= 1) {
        for (i = gap; i < n; i++) {
            int current = data[i];
            int j = i;
            while (j >= gap) {
                comparisons++;
                if (data[j - gap] > current) {
                    data[j] = data[j - gap];
                    j -= gap;
                } else {
                    break;
                }
            }
            data[j] = current;
        }
        gap /= 3;
    }
    return comparisons;
}

static int is_sorted(const int data[], int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (data[i - 1] > data[i]) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    const int sizes[SIZE_COUNT] = {1000, 2000, 4000, 8000, 16000, 20000};
    unsigned int seed = (argc >= 2) ? (unsigned int)strtoul(argv[1], NULL, 10)
                                    : (unsigned int)time(NULL);
    int s;
    srand(seed);

    printf("Random seed : %u\n", seed);
    printf("Value range : 0 ~ %d\n\n", VALUE_MAX);

    for (s = 0; s < SIZE_COUNT; s++) {
        int n = sizes[s];
        int *original = (int *)malloc(sizeof(int) * (size_t)n);
        int *a = (int *)malloc(sizeof(int) * (size_t)n);
        int *b = (int *)malloc(sizeof(int) * (size_t)n);
        int *c = (int *)malloc(sizeof(int) * (size_t)n);
        Count ca, cb, cc;

        if (!original || !a || !b || !c) {
            fprintf(stderr, "Memory allocation failed.\n");
            free(original); free(a); free(b); free(c);
            return 1;
        }

        fill_random_data(original, n);
        memcpy(a, original, sizeof(int) * (size_t)n);
        memcpy(b, original, sizeof(int) * (size_t)n);
        memcpy(c, original, sizeof(int) * (size_t)n);

        ca = insertion_sort(a, n);
        cb = shell_sort_original(b, n);
        cc = shell_sort_knuth(c, n);

        printf("Data Size : %d\n", n);
        printf("Insertion Sort        : %lld comparisons\n", ca);
        printf("Shell Sort (Original) : %lld comparisons\n", cb);
        printf("Shell Sort (Knuth)    : %lld comparisons\n", cc);
        printf("Sorted Verification   : %s\n\n",
               (is_sorted(a, n) && is_sorted(b, n) && is_sorted(c, n)) ? "Yes" : "No");

        free(original); free(a); free(b); free(c);
    }
    return 0;
}
