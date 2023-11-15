#include "batcher.h"

#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void comparator(
    Stats *stats, uint32_t A[], uint32_t x, uint32_t y) { //compares x to y and swaps accordingly
    if (cmp(stats, A[x], A[y]) > 0) { //A[x] > A[y]
        swap(stats, &A[x], &A[y]); //swaps A[x] with A[y]
    }
}

uint32_t bit_length(uint32_t n) { //returns number of bits in n
    uint32_t t = 0;
    while (n > 0) { //loops through n
        n >>= 1; //right shifts n bits
        t += 1; //increment t counter
    }
    return t;
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) { //batcher sort function
    if (n == 0) { //checks if n is not 0
        return;
    }
    uint32_t t = 0;
    t = bit_length(n); //calls bit length function to return bit length of n
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = (1 << (t - 1));
        uint32_t r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (uint32_t i = 0; i < (n - d); i += 1) {
                if ((i & p) == r) {
                    comparator(stats, A, i, (i + d));
                }
            }
            d = (q - p);
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
