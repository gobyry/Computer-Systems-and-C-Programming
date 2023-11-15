#include "batcher.h"
#include "heap.h"
#include "math.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "ahbsqr:n:p:H"

void print_help(void) { //program usage function
    fprintf(stderr, "SYNOPSIS\n"
                    "   A collection of comparison-based sorting algorithms.\n"
                    "\n"
                    "USAGE\n"
                    "   ./sorting [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -H              Display program help and usage.\n"
                    "   -a              Enable all sorts.\n"
                    "   -s              Enable Shell Sort.\n"
                    "   -b              Enable Batcher Sort.\n"
                    "   -h              Enable Heap Sort.\n"
                    "   -q              Enable Quick Sort.\n"
                    "   -n length       Specify number of array elements (default: 100).\n"
                    "   -p elements     Specify number of elements to print (default: 100).\n"
                    "   -r seed         Specify random seed (default: 13371453).\n");
}

//start
int main(int argc, char **argv) {
    int opt = 0;
    char *end;
    uint64_t seed = 13371453; //default seed
    uint64_t size = 100; //default size
    uint64_t elements = 100; //default elements
    bool h = false; //print statement checker
    Set s = set_empty();

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': //run all
            s = set_insert(s, 1);
            s = set_insert(s, 2);
            s = set_insert(s, 3);
            s = set_insert(s, 4);
            break;
        case 's': //shell sort
            s = set_insert(s, 1);
            break;
        case 'b': //batcher sort
            s = set_insert(s, 2);
            break;
        case 'q': //quick sort
            s = set_insert(s, 4);
            break;
        case 'h': //heap sort
            s = set_insert(s, 3);
            break;
        case 'r': //random seed
            seed = strtoul(optarg, &end, 10);
            break;
        case 'n': //# of elements
            size = atoi(optarg);
            if (1 <= size && size <= 10000000) { //min max size
                continue;
            } else {
                return 1;
                break;
            }
            break;
        case 'p': //print elements
            elements = atoi(optarg);
            break;
        case 'H': //print program usage
            h = true;
            break;
        case '?': //invalid print program usage
            h = true;
            break;
        default: //default print program usage
            h = true;
            break;
        }
    }
    Stats stats;
    reset(&stats);
    void *arr = calloc(size, sizeof(uint32_t)); //create test array
    uint32_t *test_arr = (uint32_t *) arr;

    if (h == true) { //prints program usage
        print_help();
        return 1; //ends program so other commands aren't taken
    }
    if (set_member(s, 1)) { //print shell
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            test_arr[i] = random() & 0x3FFFFFFF;
        }
        shell_sort(&stats, test_arr, size);
        printf("Shell Sort, %lu elements, %lu moves, %lu compares\n", size,
            stats.moves, //prints sorted array and stats
            stats.compares);
        uint32_t x;
        if (size < elements) { //if size smaller than elements, print specified array size
            x = size;
        } else {
            x = elements;
        }
        for (uint32_t i = 0; i < x; i += 1) {
            printf("%13" PRIu32 "", test_arr[i]);
            if (((i + 1) % 5 == 0)
                || (i
                    == x - 1)) { //if elements exceed 5 print new line, if last element, print new line
                printf("\n");
            }
        }
        reset(&stats);
    }
    if (set_member(s, 2)) { //print batcher
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            test_arr[i] = random() & 0x3FFFFFFF;
        }
        batcher_sort(&stats, test_arr, size);
        printf("Batcher Sort, %lu elements, %lu moves, %lu compares\n", size,
            stats.moves, //prints sorted array and stats

            stats.compares);
        uint32_t x;
        if (size < elements) { //if size smaller than elements, print specified array size
            x = size;
        } else {
            x = elements;
        }
        for (uint32_t i = 0; i < x; i += 1) {
            printf("%13" PRIu32 "", test_arr[i]);
            if (((i + 1) % 5 == 0)
                || (i
                    == x - 1)) { //if elements exceed 5 print new line, if last element, print new line
                printf("\n");
            }
        }
        reset(&stats);
    }
    if (set_member(s, 3)) { //print heap sort
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            test_arr[i] = random() & 0x3FFFFFFF;
        }
        heap_sort(&stats, test_arr, size);
        printf("Heap Sort, %lu elements, %lu moves, %lu compares\n", size,
            stats.moves, //prints sorted array and stats

            stats.compares);
        uint32_t x;
        if (size < elements) { //if size smaller than elements, print specified array size
            x = size;
        } else {
            x = elements;
        }
        for (uint32_t i = 0; i < x; i += 1) {
            printf("%13" PRIu32 "", test_arr[i]);
            if (((i + 1) % 5 == 0)
                || (i
                    == x - 1)) { //if elements exceed 5 print new line, if last element, print new line
                printf("\n");
            }
        }
        reset(&stats);
    }
    if (set_member(s, 4)) { //print quick
        srandom(seed);
        for (uint32_t i = 0; i < size; i += 1) {
            test_arr[i] = random() & 0x3FFFFFFF;
        }
        quick_sort(&stats, test_arr, size);
        printf("Quick Sort, %lu elements, %lu moves, %lu compares\n", size,
            stats.moves, //prints sorted array and stats
            stats.compares);
        uint32_t x;
        if (size < elements) { //if size smaller than elements, print specified array size
            x = size;
        } else {
            x = elements;
        }
        for (uint32_t i = 0; i < x; i += 1) {
            printf("%13" PRIu32 "", test_arr[i]);
            if (((i + 1) % 5 == 0)
                || (i
                    == x - 1)) { //if elements exceed 5 print new line, if last element, print new line
                printf("\n");
            }
        }
        reset(&stats);
    }

    free(test_arr); //frees memory
    return 0;
}
