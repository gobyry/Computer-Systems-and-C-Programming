#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

extern gmp_randstate_t state;

void gcd(mpz_t g, mpz_t a, mpz_t b) { //returns the greatest common divisor
    mpz_t tmp, tmp_a, tmp_b; //declare temps
    mpz_inits(tmp, tmp_a, tmp_b, NULL);

    mpz_set(tmp_a, a); //set temps
    mpz_set(tmp_b, b);

    while (mpz_cmp_ui(tmp_b, 0) != 0) { //b != 0
        mpz_set(tmp, tmp_b); //temp = b
        mpz_mod(tmp_b, tmp_a, tmp_b); //b = a % b
        mpz_set(tmp_a, tmp); //temp a = temp
    }
    mpz_set(g, tmp_a); //d equal to a
    mpz_clears(tmp, tmp_a, tmp_b, NULL); //clear temps
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) { //returns inverse of a mod number

    mpz_t r, rr, t, tt, q, tmp, tmp_2; //declare vars

    mpz_init_set(r, n); //r = n
    mpz_init_set(rr, a); //r' = a

    mpz_init_set_ui(t, 0); //t = 0
    mpz_init_set_ui(tt, 1); //t' = 0

    mpz_init_set_ui(q, 0); //q = 0

    mpz_inits(tmp, tmp_2, NULL); //initialize temps

    while (mpz_cmp_d(rr, 0) != 0) {
        mpz_fdiv_q(q, r, rr); //q = r/r'

        mpz_set(tmp, r); //tmp = r
        mpz_set(r, rr); //r = r'
        mpz_mul(rr, q, rr); //r' = q * r'
        mpz_sub(rr, tmp, rr); //r' = temp - r'

        mpz_set(tmp_2, t); //tmp_2 = t
        mpz_set(t, tt); //t' = t
        mpz_mul(tt, q, tt); //t' = q * t'
        mpz_sub(tt, tmp_2, tt); //t' = temp - t'
    }
    if (mpz_cmp_d(r, 1) > 0) { //r > 1
        mpz_set_ui(o, 0); // o = 0 (no inverse)
        mpz_clears(r, rr, t, tt, q, tmp, tmp_2, NULL); //clears
        return;
    }
    if (mpz_cmp_d(t, 0) < 0) { //t < 0
        mpz_add(t, t, n); //t = t + n
    }
    mpz_set(o, t); //o = t
    mpz_clears(r, rr, t, tt, q, tmp, tmp_2, NULL); //clears
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) { //calculates based raised to exponent power mod

    mpz_t v, p, tmp_v, tmp_p, tmp_d; //declare vars

    mpz_init_set_ui(v, 1); //v = 1
    mpz_init_set(p, a); //p = a

    mpz_inits(tmp_v, tmp_p, NULL);
    mpz_init_set(tmp_d, d);

    while (mpz_cmp_d(tmp_d, 0) > 0) { //while d > 0
        if (mpz_odd_p(tmp_d) != 0) { //if d is odd
            mpz_mul(tmp_v, v, p); //v = v * p
            mpz_mod(v, tmp_v, n); //v = temp v % n
        }
        mpz_mul(tmp_p, p, p); //temp p = p * p
        mpz_mod(p, tmp_p, n); //p = temp p % n

        mpz_fdiv_q_ui(tmp_d, tmp_d, 2); //temp d = temp d / 2
    }
    mpz_set(o, v); //o = v
    mpz_clears(v, p, tmp_v, tmp_p, tmp_d, NULL); //clears
}

bool is_prime(mpz_t n, uint64_t iters) { //checks if prime

    if ((mpz_cmp_d(n, 0) == 0) || ((mpz_cmp_d(n, 1) == 0))
        || ((mpz_cmp_d(n, 4) == 0))) { //0,1,4 are not prime
        return false;
    }
    if ((mpz_cmp_d(n, 2) == 0) || ((mpz_cmp_d(n, 3) == 0)) || ((mpz_cmp_d(n, 5) == 0))
        || ((mpz_cmp_d(n, 5) == 0))) { //2,3,5 are prime
        return true;
    }

    if (mpz_cmp_d(n, 2) < 0) { //n < 2 is not prime
        return false;
    }

    mpz_t a, y, j, r, s, two, n_min1, n_min3, s_min1;
    mpz_inits(a, y, j, r, s, two, n_min1, n_min3, s_min1, NULL);

    mpz_set_ui(two, 2); //two = 2
    mpz_sub_ui(n_min1, n, 1); //n - 1
    mpz_sub_ui(n_min3, n, 3); //n - 3
    mpz_sub_ui(s_min1, s, 1); //s - 1

    mpz_set(r, n_min1); //r = n - 1
    mpz_set_ui(s, 0); //s = 0

    while (mpz_odd_p(r)) { //r is not odd
        mpz_fdiv_q_ui(r, r, 2); //set r equal to r / 2
        mpz_add_ui(s, s, 1); //s+=1
    }

    for (uint64_t i = 1; i < iters; i += 1) { //i to k
        mpz_urandomm(a, state, n_min3); //choose random number
        mpz_add_ui(a, a, 2); //a = a + 2
        pow_mod(y, a, r, n); //pow mod (a, r, n)
        if ((mpz_cmp_d(y, 1) != 0) && (mpz_cmp(y, n_min1) != 0)) { //y != 1
            mpz_set_ui(j, 1); //j = 1
            while ((mpz_cmp(j, s_min1) <= 0) && (mpz_cmp(y, n_min1) != 0)) {
                pow_mod(y, y, two, n); //pow mod (y, 2, n)
                if (mpz_cmp_d(y, 1) == 0) { //y = 1
                    mpz_clears(a, y, j, r, s, n_min1, n_min3, s_min1, two, NULL); //clears
                    return false;
                }
                mpz_add_ui(j, j, 1); //j+=1
            }
            if (mpz_cmp(y, s_min1) != 0) { //y != n - 1
                mpz_clears(a, y, j, r, s, n_min1, n_min3, s_min1, two, NULL);
                return false;
            }
        }
    }
    mpz_clears(a, y, j, r, s, n_min1, n_min3, s_min1, two, NULL); //clears
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) { //generates new prime number
    mpz_t tmp; //declare vars
    mpz_init(tmp); //intialize vars
    bool value = false; //boolean value is false

    while (value == false) { //while value is false
        mpz_rrandomb(tmp, state, bits); //generate random number and set to tmp
        if (is_prime(tmp, iters)) { //is prime tmp and iters
            value = true; //quit loop
        }
    }
    mpz_set(p, tmp); //p = temp
    mpz_clear(tmp); //clear temp
}
