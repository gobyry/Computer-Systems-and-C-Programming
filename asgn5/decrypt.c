#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <gmp.h>
#include <math.h>
#include "ss.h"
#include "numtheory.h"
#include "randstate.h"

#define OPTIONS "i:o:n:vh"

extern gmp_randstate_t state;

void print_help(void) { //print program usage
    fprintf(stderr, "SYNOPSIS\n"
                    "   Decrypts data using SS decryption.\n"
                    "   Encrypted data is encrypted by the encrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "   ./decrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to decrypt (default: stdin).\n"
                    "   -o outfile      Output file for decrypted data (default: stdout).\n"
                    "   -n pvfile       Private key file (default: ss.priv).\n");
}

int main(int argc, char **argv) {
    int opt = 0;

    //FILE
    FILE *pvfile;
    FILE *infile = NULL;
    FILE *outfile = NULL;

    //char
    char *in = NULL;
    char *out = NULL;
    char *pv = "ss.priv";

    //verbose
    bool verbose = false;
    bool print = true;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': //specifies input file
            print = false;
            in = optarg;
            break;

        case 'o': //specifies output file
            print = false;
            out = optarg;
            break;

        case 'n': //specifies pvfile
            print = false;
            pv = optarg;
            break;

        case 'v': //enables verbose
            print = false;
            verbose = true;
            break;

        case 'h': //program usage
            print = true;
            break;
        }
    }
    if (argc == 1) { //./decrypt with no arguments
        print = false;
    }

    if (print == true) { //print program usage
        print_help();
        return 1;
    }

    if (in == NULL) { //null input
        infile = stdin; //default
    } else {
        infile = fopen(in, "r"); //open infile
        if (!infile) { //error
            fclose(infile);
            printf("Unable to open input file.\n");
            return 1;
        }
    }

    if (out == NULL) { //null output
        outfile = stdout; //default
    } else {
        outfile = fopen(out, "w"); //open outfile
        if (!outfile) { //error
            fclose(outfile);
            printf("Unable to open output file.\n");
            return 1;
        }
    }

    pvfile = fopen(pv, "r"); //open pvfile
    if (!pvfile) {
        fclose(pvfile); //causes seg fault
        printf("Unable to open private file.\n");
        return 1;
    }
    mpz_t pq, d;
    mpz_inits(pq, d);

    ss_read_priv(pq, d, pvfile); //read private key

    size_t pq_bits, d_bits;
    pq_bits = mpz_sizeinbase(pq, 2);
    d_bits = mpz_sizeinbase(d, 2);

    if (verbose == true) { //verbose enabled
        gmp_fprintf(stderr, "pq (%zu bits) = %Zd\n", pq_bits, pq); //private modulus pq
        gmp_fprintf(stderr, "d  (%zu bits) = %Zd\n", d_bits, d); //private key d
    }

    ss_decrypt_file(infile, outfile, d, pq); //decrypt infile

    fclose(infile); //close infile
    fclose(outfile); //close outfile
    fclose(pvfile); //close pvfile

    mpz_clears(pq, d, NULL);

    return 0;
}
