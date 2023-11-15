#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

extern gmp_randstate_t state;

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) { //makes public key

    mpz_t p_min1, q_min1, ans, pq; //declare vars
    mpz_inits(p_min1, q_min1, ans, pq, NULL); //intialize vars

    uint64_t min, max, pbits, qbits, diff; //declare uints

    min = (nbits / 5); //checks min in range
    max = ((2 * nbits) / 5); //checks max in range
    diff = (max - min); //difference between min and max

    pbits = (min + (random() % (diff))); //sets p bits
    qbits = (nbits - 2 * pbits); //sets q bits

    make_prime(p, pbits, iters); //make prime p
    make_prime(q, qbits, iters); //make prime q

    mpz_mul(n, p, p); //n equals p * p
    mpz_mul(n, n, q); //n equals p * p * q

    mpz_sub_ui(p_min1, p, 1); //p - 1
    mpz_sub_ui(q_min1, q, 1); //q - 1

    mpz_clears(p_min1, q_min1, ans, pq, NULL); //clears
}

void ss_make_priv(mpz_t d, mpz_t pq, mpz_t p, mpz_t q) { //make private key

    mpz_t p_min1, q_min1, ans, lam, n, tmp; //declare vars
    mpz_inits(p_min1, q_min1, ans, lam, n, tmp, NULL); //intialize vars
    mpz_sub_ui(p_min1, p, 1); //p = p - 1
    mpz_sub_ui(q_min1, q, 1); // q= q - 1

    //calculate lcm lambda
    gcd(ans, p_min1, q_min1); //ans = gcd(p - 1, q - 1)

    mpz_mul(tmp, p_min1, q_min1); //(pq = p - 1)(q - 1)
    mpz_mul(pq, p, q); //pq = p * q
    mpz_fdiv_q(lam, tmp, ans); //lambda = tmp/gcd(p - 1, q - 1)

    mpz_mul(n, p, p); //n equals p * p
    mpz_mul(n, n, q); //n equals p * p * q

    mod_inverse(d, n, lam); //d = mod inverse (n, lam)

    mpz_clears(p_min1, q_min1, lam, ans, n, tmp, NULL); //clears
}

void ss_write_pub(mpz_t n, char username[], FILE *pbfile) { //writes public key
    gmp_fprintf(pbfile, "%Zx\n", n); //writes n
    gmp_fprintf(pbfile, "%s\n", username); //writes username
}

void ss_write_priv(mpz_t pq, mpz_t d, FILE *pvfile) { //writes private key
    gmp_fprintf(pvfile, "%Zx\n", pq); //writes pq
    gmp_fprintf(pvfile, "%Zx\n", d); //writes d
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) { //reads public key
    gmp_fscanf(pbfile, "%Zx\n", n); //reads n
    gmp_fscanf(pbfile, "%s\n", username); //readers username
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) { //reads private key
    gmp_fscanf(pvfile, "%Zx\n", pq); //reads pq
    gmp_fscanf(pvfile, "%Zx\n", d); //reads d
}

void ss_encrypt(mpz_t c, mpz_t m, mpz_t n) { //ss encrypt message using public key
    pow_mod(c, m, n, n); //c = m^n(mod n)
}

void ss_encrypt_file(FILE *infile, FILE *outfile, mpz_t n) { //encrypts infile

    mpz_t msg, ans; //declare vars
    mpz_inits(msg, ans, NULL); //intialize vars

    size_t size = mpz_sizeinbase(n, 2); //size = log
    size = size * 0.5; //log^2
    size = ((size - 1) / 8); //size = (log^2n - 1)/8

    uint8_t *arr = (uint8_t *) malloc(size); //block array

    arr[0] = 0xFF; //0 out byte

    while (!feof(infile)) { //unprocessed bytes
        size_t k = fread(arr + 1, sizeof(uint8_t), size - 1, infile); //read at k - 1
        mpz_import(msg, k + 1, 1, sizeof(uint8_t), 1, 0, arr); //convert bytes into mpz msg
        ss_encrypt(ans, msg, n); //encrypt message answer
        gmp_fprintf(outfile, "%Zx\n", ans); //print encrypted number as hexstring
    }
    mpz_clears(msg, ans, NULL); //clear vars
    free(arr); //free array
}

void ss_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t pq) { //ss decrypt message using private key
    pow_mod(m, c, d, pq); //m = c^d(mod pq)
}

void ss_decrypt_file(FILE *infile, FILE *outfile, mpz_t d, mpz_t pq) { //decrypts infile

    mpz_t msg, ans; //declare vars
    mpz_inits(msg, ans, NULL); //initialize vars

    size_t size = mpz_sizeinbase(pq, 2); //size = log
    size = ((size - 1) / 8); //size = (logn - 1)/8

    //uint8_t *arr = (uint8_t*) calloc(size, sizeof(uint8_t));
    uint8_t *arr = (uint8_t *) malloc(size * sizeof(uint8_t)); //block array

    while (!feof(infile)) { //unprocessed bytes
        gmp_fscanf(infile, "%Zx\n", msg); //read hexstring
        ss_decrypt(ans, msg, d, pq); //decrypts message answer
        mpz_export(arr, &size, 1, sizeof(uint8_t), 1, 0, ans); //exports
        fwrite(arr + 1, size - 1, sizeof(uint8_t), outfile); //writes into outfile
    }
    mpz_clears(msg, ans, NULL); //clear vars
    free(arr); //free array
}
