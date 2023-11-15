#include "mathlib.h"

#include <stdio.h>

static long double term = 1.0;
static int counter = 0;

double pi_bbp(void) { //returns pi value from bbp method
    double num = 0;
    double den = 0;
    double pi = 0;
    double exp = 1;
    for (double k = 0; term > EPSILON; k += 1.0) {
        if (k == 0) { //if k equals 0, 16^0 will be 1
            num = (k * (120 * k + 151) + 47); //calculation for numerator
            den = (k * (k * (k * (512 * k + 1024) + 712) + 194) + 15); //calculation for denominator
            exp = 1; //sets 16^0 equal to 1
            term = (1 / exp) * (num / den); //term calculation
            pi += term; //adds term approximation to pi
            counter += 1; //increment counter

        } else { //if k isn't 0
            num = (k * (120 * k + 151) + 47); //calculation for numerator
            den = (k * (k * (k * (512 * k + 1024) + 712) + 194) + 15); //calculation for denominator
            exp *= (16); //16^k
            term = (1 / exp) * (num / den); //term calculation
            pi += term; //adds term approximation to pi
            counter += 1; //increment counter
        }
    }
    return pi;
}

int pi_bbp_terms(void) { //returns bbp terms
    return counter;
}
