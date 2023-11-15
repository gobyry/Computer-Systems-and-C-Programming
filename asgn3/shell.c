#include "shell.h"

#include "gaps.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) { //shell sort
    uint32_t j = 0;
    uint32_t temp = 0;
    for (uint32_t x = 0; x < GAPS; x += 1) { //for gap in gaps
        uint32_t gap = gaps[x];
        for (uint32_t i = gap; i < length; i += 1) { //loops through gap
            j = i;
            temp = arr[j];
            move(stats, arr[j]); //moves
            while ((j >= gap)
                   && (cmp(stats, temp, arr[j - gap]) < 0)) { //compare temp array to arr[j-gap]
                arr[j] = arr[j - gap];
                move(stats, arr[j - gap]); //moves
                j -= gap;
            }
            arr[j] = temp; //a move
            move(stats, arr[j]);
        }
    }
}
