#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "trie.h"
#include "word.h"
#include "io.h"
#include "code.h"

#define OPTIONS "vi:o:h"

void print_help(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Decompresses files with the LZ78 decompression algorithm.\n"
                    "   Used with files compressed with the corresponding encoder.\n"
                    "\n"
                    "USAGE\n"
                    "   ./decode [-vh] [-i input] [-o output]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -v          Display decompression statistics\n"
                    "   -i input    Specify input to decompress (stdin by default)\n"
                    "   -o output   Specify output of decompressed input (stdout by default)\n"
                    "   -h          Display program usage\n");
}

uint32_t bit_length(uint32_t n) { //returns number of bits in n
    uint32_t t = 0;
    while (n > 0) { //loops through n
        t += 1; //increment t
        n >>= 1; //right shifts n bits
    }
    return t;
}

int main(int argc, char **argv) {
    int opt = 0;

    int infile = 0;
    int outfile = 0;

    char *in = NULL;
    char *out = NULL;

    bool print_stats = false;
    bool print = true;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'v':
            print_stats = true;
            print = false;
            break;

        case 'i':
            in = optarg;
            print = false;
            break;

        case 'o':
            out = optarg;
            print = false;
            break;
        }
    }
    if (argc == 1) { //./decode with no arguments
        print = false;
    }

    if (print == true) { //print program usage
        print_help();
        return 1;
    }

    if (in == NULL) { //null input
        infile = STDIN_FILENO; //default in
    } else {
        infile = open(in, O_RDONLY); //open infile
        if (infile == -1) { //error
            close(infile);
            printf("Unable to open input file.\n");
            return 1;
        }
    }

    FileHeader header; //create header struct
    read_header(infile, &header); //read header

    if (out == NULL) { //null output
        outfile = STDOUT_FILENO; //default out
    } else {
        outfile = open(out, O_WRONLY | O_TRUNC, S_IRUSR | O_CREAT | S_IWUSR); //open outfile
        if (outfile == -1) { //error
            close(outfile);
            printf("Unable to open output file.\n");
            return 1;
        }
    }
    WordTable *table = wt_create(); //create word table

    uint16_t curr_code = 0; //set current code
    uint16_t next_code = START_CODE; //set next code to start code
    uint8_t curr_sym = 0; //set current symbols

    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code))) { //loop through infile
        table[next_code] = word_append_sym(table[curr_code], curr_sym); //set table
        write_word(outfile, table[next_code]); //write words to outfile
        next_code += 1; //increment next code by 1

        if (next_code == MAX_CODE) { //if next code equals max code
            wt_reset(table); //reset word table
            next_code = START_CODE; //set next code equal to start code
        }
    }
    flush_words(outfile); //flush words to outfile

    if (print_stats == true) { //if print stats
        uint64_t cb = (total_bits); //calculates compressed bytes
        uint64_t ub = (total_syms); //calculate uncompressed bytes
        double ratio = 100 * (1 - ((double) ub / cb)); //calculates ratio

        printf("Compressed file size: %lu bytes\n", ub); //print compressed file size
        printf("Uncompressed file size: %lu bytes\n", cb); //print uncompressed file size
        printf("Compression ratio: %.2f%%\n", ratio); //print compression ratio
    }

    close(infile); //close infile
    close(outfile); //close outfile

    wt_delete(table); //delete word table

    return 0;
}
