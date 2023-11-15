#include "quick.h"

#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int partition(Stats *stats, uint32_t A[], uint32_t lo,
    uint32_t hi) { //rearange values depending on < or > to pivot value
    uint32_t i = (lo - 1);
    for (uint32_t j = lo; j < hi; j += 1) {
        if (cmp(stats, A[j - 1], A[hi - 1]) < 0) { //compares A[j-1] < A[hi-1]
            i += 1;
            swap(stats, &A[j - 1], &A[i - 1]); //swaps A[i] and A[j]
        }
    }
    swap(stats, &A[i], &A[hi - 1]); //swaps A[i] and A[hi - 1]
    return (i + 1);
}

void quick_sorter(
    Stats *stats, uint32_t A[], uint32_t lo, uint32_t hi) { //recursive function for quicksort
    int p = 0;
    if (lo < hi) {
        p = partition(stats, A, lo, hi); //partitions arrays
        quick_sorter(stats, A, lo, p - 1); //quick through low values
        quick_sorter(stats, A, p + 1, hi); //quick sort through high values
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) { //quick sort function
    quick_sorter(stats, A, 1, n);
}
