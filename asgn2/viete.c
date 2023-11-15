#include "mathlib.h"

#include <stdio.h>

static int counter = 0;

double pi_viete(void) { //returns pi approximation using viete method
    double term = sqrt_newton(2.0); //set nested sqrt(2)
    double pi = (sqrt_newton(2.0) / 2); //pi equals sqrt(2)/2 at first term
    double pre_pi = 0;
    while (absolute(pi - pre_pi) > EPSILON) { //loop while term is greater than epsilon
        term = sqrt_newton(2 + term); //nested sqrt(2)
        pre_pi = pi; //sets previous pi term equal to pi
        pi *= (term * 0.5); //pi equals
        counter += 1; //increment counter
    }
    pi = 1 / (pi * 0.5); //make 2/pi to pi
    return pi;
}

int pi_viete_factors(void) { //returns number of factors
    return counter;
}
