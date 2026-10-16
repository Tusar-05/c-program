#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Sorting Algorithms Benchmark
// Compares 3 hand-written O(n^2) sorts against the standard library qsort (O(n log n)).
// Each algorithm sorts an identical copy of the same random array, timed with clock().

// Usage:
//   ./sort_benchmark          -> runs the default sizes (1000, 5000, 10000, 20000)
//   ./sort_benchmark 50000    -> runs a single custom size instead

#define MAX_N 100000
#define MIN_SIZE 10
#define SEED 42
#define VALUE_RANGE 100000

// ---------- Helpers ----------

// Fill an array with random integers in [0, 99999]
void fill_random(int *a, int n, unsigned seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        a[i] = rand() % VALUE_RANGE;
    }
}

// Return 1 if array is sorted in non-decreasing order, else 0
int is_sorted(const int *a, int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) return 0;
    }
    return 1;
}

// ---------- O(n^2) sorts ----------

// Bubble Sort: repeatedly swap adjacent out-of-order pairs.
// Early exit if a full pass makes no swaps (array became sorted).
void bubble_sort(int *a, int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;   // already sorted
    }
}

// Selection Sort: find the minimum in the unsorted tail, swap it into place.
void selection_sort(int *a, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            int tmp = a[i];
            a[i] = a[min_idx];
            a[min_idx] = tmp;
        }
    }
}

// Insertion Sort: grow a sorted prefix by inserting each next element
// into its correct position (like sorting cards in your hand).
void insertion_sort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

// ---------- qsort comparator ----------

int compare_ints(const void *x, const void *y) {
    int a = *(const int *)x;
    int b = *(const int *)y;
    return (a > b) - (a < b);
}

// ---------- Benchmark driver ----------

// Runs one algorithm on a fresh copy of the source array, returns elapsed
// milliseconds (as double). Verifies the result is actually sorted.
double benchmark(void (*sort_fn)(int *, int), const int *src, int n, const char *name) {
    static int buffer[MAX_N];
    memcpy(buffer, src, n * sizeof(int));

    clock_t start = clock();
    sort_fn(buffer, n);
    clock_t end = clock();

    double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    if (!is_sorted(buffer, n)) {
        printf("  !! %s produced an UNSORTED result!\n", name);
    }
    return ms;
}

// Same as above but for qsort, which has a different function signature.
double benchmark_qsort(const int *src, int n) {
    static int buffer[MAX_N];
    memcpy(buffer, src, n * sizeof(int));

    clock_t start = clock();
    qsort(buffer, n, sizeof(int), compare_ints);
    clock_t end = clock();

    double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    if (!is_sorted(buffer, n)) {
        printf("  !! qsort produced an UNSORTED result!\n");
    }
    return ms;
}

int main(int argc, char **argv) {
    int default_sizes[] = {1000, 5000, 10000, 20000};
    const int *sizes = default_sizes;
    int num_sizes = sizeof(default_sizes) / sizeof(default_sizes[0]);
    int custom_size = 0;
    static int data[MAX_N];

    if (argc == 2) {
        custom_size = atoi(argv[1]);
        if (custom_size < MIN_SIZE || custom_size > MAX_N) {
            fprintf(stderr, "Size must be between %d and %d\n", MIN_SIZE, MAX_N);
            return 1;
        }
        sizes = &custom_size;
        num_sizes = 1;
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [size]\n", argv[0]);
        return 1;
    }

    printf("==============================================\n");
    printf("   Sorting Algorithms Benchmark\n");
    printf("==============================================\n");
    printf("Each algorithm sorts the SAME random array.\n");
    printf("Fixed seed %d => identical data on every run.\n", SEED);
    printf("Times in milliseconds (lower is better).\n\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        fill_random(data, n, SEED);

        printf("--- n = %d (random data) ---\n", n);
        printf("  %-15s %10s\n", "Algorithm", "Time (ms)");
        printf("  %-15s %10.2f\n", "Bubble",    benchmark(bubble_sort,    data, n, "Bubble"));
        printf("  %-15s %10.2f\n", "Selection", benchmark(selection_sort, data, n, "Selection"));
        printf("  %-15s %10.2f\n", "Insertion", benchmark(insertion_sort, data, n, "Insertion"));
        printf("  %-15s %10.2f\n", "qsort",     benchmark_qsort(data, n));
        printf("\n");
    }

    // Best-case check: on ALREADY SORTED data insertion sort becomes O(n)
    // (one comparison per element, zero swaps), while selection sort stays
    // O(n^2) no matter what.
    int n = (custom_size > 0 ? custom_size : 10000);
    for (int i = 0; i < n; i++) data[i] = i;

    printf("--- n = %d (already sorted / best case) ---\n", n);
    printf("  %-15s %10s\n", "Algorithm", "Time (ms)");
    printf("  %-15s %10.2f\n", "Bubble",    benchmark(bubble_sort,    data, n, "Bubble"));
    printf("  %-15s %10.2f\n", "Selection", benchmark(selection_sort, data, n, "Selection"));
    printf("  %-15s %10.2f\n", "Insertion", benchmark(insertion_sort, data, n, "Insertion"));
    printf("  %-15s %10.2f\n", "qsort",     benchmark_qsort(data, n));
    printf("\n");

    printf("==============================================\n");
    printf("Observations:\n");
    printf(" - Bubble/Selection/Insertion are O(n^2): doubling\n");
    printf("   n roughly quadruples their time.\n");
    printf(" - qsort is O(n log n): it stays fast even at large n.\n");
    printf(" - Insertion sort beats the other O(n^2) sorts on\n");
    printf("   random data thanks to fewer swaps and good cache use.\n");
    printf(" - On already-sorted data insertion sort drops to O(n)\n");
    printf("   (nearly instant), while selection sort does not care.\n");
    printf("==============================================\n");

    return 0;
}
