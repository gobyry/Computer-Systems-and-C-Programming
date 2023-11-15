#include "mathlib.h"

#include <stdio.h>

static long double term = 1.0;
static int counter = 0;

double pi_euler(void) { //returns pi value from euler method
    double pi = 0; //pi value
    double sum = 0; //sum value
    for (double k = 1.0; term > EPSILON; k += 1.0) { //loops until term value is less than epsilon
        term = (1 / (k * k)); //1/k^2 terms
        sum += term; //summation of terms
        counter += 1;
    }
    pi = sqrt_newton(6 * sum); //(pi^2)/6 = sqrt(6 * (1/k^2))
    return pi;
}

int pi_euler_terms(void) { //returns number of terms
    return counter;
}
