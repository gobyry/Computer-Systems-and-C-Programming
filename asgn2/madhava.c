#include "mathlib.h"

#include <stdio.h>

static long double term = 1.0;
static int counter = 0;

double pi_madhava(void) { //returns pi approximation with madhava method
    double pi = 0;
    double num = 0;
    double den = 0;
    double exp = 1;
    for (double k = 0; absolute(term) > EPSILON; k += 1.0) { //loop until term is less than EPSILON
        if (k == 0) { //first term of the sum
            exp = 1; //-3^0 = -1
            num = 1;
            den = ((exp) * (2 * k + 1)); //calculation for denominator ((1/-1) * (2*0+1))
            term = (num / den); // (1/ ((-1) * (2*0+1))
            pi += term;
            counter += 1;
        } else {
            exp *= (-3); //-3^k
            num = 1;
            den = ((exp) * (2 * k + 1)); //((-3^k) * (2*k+1))
            term = (num / den);
            pi += term;
            counter += 1;
        }
    }
    pi *= sqrt_newton(12); //pi/sqrt(12) to pi
    return pi;
}

int pi_madhava_terms(void) { //returns number of terms
    return counter;
}
