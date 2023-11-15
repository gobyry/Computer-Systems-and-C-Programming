#include "mathlib.h"

#include <stdio.h>

static int counter = 0;

double sqrt_newton(double x) { //sqrt function from python pseudo in asgn2
    double y = 1.0;
    double z = 0.0;
    counter = 0;
    while (absolute(y - z) > EPSILON) { //loops value 1 minus 2 is greater than epsilon
        z = y;
        y = 0.5 * (z + x / z);
        counter += 1;
    }
    return y;
}

int sqrt_newton_iters(void) { //returns iters of sqrt_newton
    return counter;
}
