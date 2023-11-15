#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gmp.h>
#include "randstate.h"

gmp_randstate_t state; //initialize state

void randstate_init(uint64_t seed) {
    srandom(seed); //set seed
    gmp_randinit_mt(state); //initialize state
    gmp_randseed_ui(state, seed); //set state to seed
}

void randstate_clear(void) {
    gmp_randclear(state); //clear state
}
