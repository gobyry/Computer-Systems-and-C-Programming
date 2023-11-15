#include "mathlib.h"

#include <stdio.h>

static long double term = 1.0;
static int counter = 0;

double e(void) { //returns eulers number
    double e = 1; //setting default e value
    double x = 1.0; //setting default x value
    for (double k = 1.0; term > EPSILON; k += 1.0) { //loop until value is less than epsilon
        term *= (x / k); //calculates k! based off previous term
        e += term; //add 1/k! to e value
        counter += 1;
    }
    return e;
}

int e_terms(void) { //returns number of terms
    return counter;
}
