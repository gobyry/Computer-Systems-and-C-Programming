#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <time.h>
#include "ss.h"
#include "numtheory.h"
#include "randstate.h"

#define OPTIONS "b:i:n:d:s:vht"

extern gmp_randstate_t state;

void print_help(void) { //program usage
    fprintf(stderr, "SYNOPSIS\n"
                    "   Generates an SS public/private key pair.\n"
                    "\n"
                    "USAGE\n"
                    "   ./keygen [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -b bits         Minimum bits needed for public key n (default: 256).\n"
                    "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n"
                    "   -n pbfile       Public key file (default: ss.pub).\n"
                    "   -d pvfile       Private key file (default: ss.priv).\n"
                    "   -s seed         Random seed for testing.\n");
}

int main(int argc, char **argv) {
    int opt = 0;
    char *end;

    //set files
    FILE *pbfile;
    FILE *pvfile;

    char *pb = "ss.pub";
    char *pv = "ss.priv";

    //seed min and max
    uint64_t nbits = 256;
    uint64_t iters = 50;
    uint64_t seed = time(NULL);
    bool verbose = false;
    bool print = true;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //get opt
        switch (opt) {
        case 'b': //min num bits 1024
            print = false;
            nbits = atoi(optarg);
            if (nbits >= 50 && nbits <= 4096) {
                continue;
            } else {
                return 1;
                break;
            }
            break;

        case 'i': //num iters 50
            print = false;
            iters = atoi(optarg);
            if (iters >= 1 && iters <= 50) {
                continue;
            } else {
                return 1;
                break;
            }
            break;

        case 'n': //specifies public key file
            print = false;
            pb = optarg;
            break;

        case 'd': //specifies private key file
            print = false;
            pv = optarg;
            break;

        case 's': //sets random seed
            print = false;
            seed = strtoul(optarg, &end, 10);
            break;

        case 'v': //enables verbose output
            print = false;
            verbose = true;
            break;

        case 'h': //program usage
            print = true;
            break;
        }
    }
    if (argc == 1) { //./keygen with no arguments
        print = false;
    }

    if (print == true) { //print program usage
        print_help();
        return 1;
    }
    mpz_t p, q, n, pq, d; //declare vars
    mpz_inits(p, q, n, pq, d, NULL); //intialize vars
    char *username = getenv("USER"); //get user name as string

    pvfile = fopen(pv, "w"); //open private file
    if (!pvfile) { //error
        printf("Unable to open file.\n");
        fclose(pvfile);
        return 1;
    }
    pbfile = fopen(pb, "w"); //open public file
    if (!pbfile) { //error
        printf("Unable to open file.\n");
        fclose(pbfile);
        fclose(pvfile);
        return 1;
    }
    fchmod(fileno(pvfile), 0600); //set file permissions to 600
    randstate_init(seed); //intialize random state

    ss_make_pub(p, q, n, nbits, iters); //make public key
    ss_make_priv(d, pq, p, q); //make private key

    ss_write_pub(n, username, pbfile); //write public key
    ss_write_priv(pq, d, pvfile); //write private key

    size_t p_bits, q_bits, n_bits, pq_bits, d_bits; //generate bit sizes
    p_bits = mpz_sizeinbase(p, 2);
    q_bits = mpz_sizeinbase(q, 2);
    n_bits = mpz_sizeinbase(n, 2);
    pq_bits = mpz_sizeinbase(pq, 2);
    d_bits = mpz_sizeinbase(d, 2);

    if (verbose == true) { //prints verbose
        gmp_printf("user = %s\n", username); //username
        gmp_printf("p  (%lu bits) = %Zd\n", p_bits, p); //first large prime
        gmp_printf("q  (%lu bits) = %Zd\n", q_bits, q); //second large prime
        gmp_printf("n  (%lu bits) = %Zd\n", n_bits, n); //public key n
        gmp_printf("pq (%lu bits) = %Zd\n", pq_bits, pq); //private exponent d
        gmp_printf("d  (%lu bits) = %Zd\n", d_bits, d); //private modulus pq
    }
    randstate_clear(); //clear state

    fclose(pbfile); //close pbfile
    fclose(pvfile); //close pvfile

    mpz_clears(p, q, n, pq, d, NULL); //clear vars

    return 0;
}
