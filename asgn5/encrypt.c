#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <gmp.h>
#include <math.h>
#include "ss.h"
#include "numtheory.h"
#include "randstate.h"

#define OPTIONS "i:o:n:vh"

extern gmp_randstate_t state;

void print_help(void) { //program usage
    fprintf(stderr, "SYNOPSIS\n"
                    "   Encrypts data using SS encryption.\n"
                    "   Encrypted data is decrypted by the decrypt program.\n"
                    "\n"
                    "USAGE\n"
                    "   ./encrypt [OPTIONS]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to encrypt (default: stdin).\n"
                    "   -o outfile      Output file for encrypted data (default: stdout).\n"
                    "   -n pbfile       Public key file (default: ss.pub).\n");
}

int main(int argc, char **argv) {
    int opt = 0;

    //FILE
    FILE *pbfile;
    FILE *infile = NULL;
    FILE *outfile = NULL;

    //char
    char *in = NULL;
    char *out = NULL;
    char *pb = "ss.pub";

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

        case 'n': //specifies public key
            print = false;
            pb = optarg;
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
    if (argc == 1) { //./encrypt with no arguments
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

    pbfile = fopen(pb, "r"); //open pb file
    if (!pbfile) {
        fclose(pbfile);
        printf("Unable to open public file.\n");
        return 1;
    }

    mpz_t n, username; //declare vars
    mpz_inits(n, username, NULL); //initialize vars

    char *str_user = getenv("USER"); //gets username
    mpz_set_str(username, str_user, 62);

    ss_read_pub(n, str_user, pbfile); //read public file

    size_t n_bits; //generate bits
    n_bits = mpz_sizeinbase(n, 2);

    if (verbose == true) { //verbose enabled
        fprintf(stderr, "user = %s\n", str_user); //username
        gmp_fprintf(stderr, "n (%zu bits) = %Zd\n", n_bits, n); //public modulus
    }
    ss_encrypt_file(infile, outfile, n); //encrypt file

    fclose(infile); //close infile
    fclose(outfile); //close outfile
    fclose(pbfile); //close pbfile
    mpz_clears(n, username, NULL); //clear vars

    return 0;
}
