#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "code.h"
#include "trie.h"
#include "word.h"
#include "io.h"

#define OPTIONS "vi:o:h"

void print_help(void) { //print help usage
    fprintf(stderr, "SYNOPSIS\n"
                    "   Compresses files using the LZ78 compression algorithm.\n"
                    "   Compressed files are decompressed with the corresponding decoder.\n"
                    "\n"
                    "USAGE\n"
                    "   ./encode [-vh] [-i input] [-o output]\n"
                    "\n"
                    "OPTIONS\n"
                    "   -v          Display compression statistics\n"
                    "   -i input    Specify input to compress (stdin by default)\n"
                    "   -o output   Specify output of compressed input (stdout by default)\n"
                    "   -h          Display program help and usage\n");
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

        case 'v': //verbose
            print = false;
            print_stats = true;
            break;

        case 'i': //infile
            in = optarg;
            print = false;
            break;

        case 'o':
            out = optarg;
            print = false;
            break;
        }
    }

    if (argc == 1) { //./encode with no arguments
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
            printf("Unable to open input file.\n"); //print error message
            return 1;
        }
    }

    if (out == NULL) { //null output
        outfile = STDOUT_FILENO; //default out
    } else {
        outfile = open(out, O_WRONLY | O_TRUNC | O_CREAT); //open outfile
        if (outfile == -1) { //error
            close(outfile);
            printf("Unable to open output file.\n"); //print error message
            return 1;
        }
    }

    struct stat stats; //create stats struct
    fstat(infile, &stats); //fstat infile
    fchmod(outfile, stats.st_mode); //fchmod outfile

    FileHeader header; //create header struct
    header.magic = MAGIC; //set magic number
    header.protection = stats.st_mode; //set mask protection
    write_header(outfile, &header); //write header

    TrieNode *tr = trie_create(); //create trie root
    tr->code = EMPTY_CODE; //code stored is empty code
    TrieNode *curr_node = tr; //copy of trie root node
    curr_node->code = EMPTY_CODE; //set copy as empty code

    uint16_t next_code = START_CODE; //monotonic counter

    TrieNode *prev_node = NULL; //create previous node
    uint8_t prev_sym = 0; //intialize previos symbol
    uint8_t curr_sym = 0; //intialize current symbol

    while (read_sym(infile, &curr_sym)) { //loop through symbols from infile
        struct TrieNode *next_node = trie_step(curr_node, curr_sym); //set next node to trie step
        if (next_node != NULL) { //if next node is not null
            prev_node = curr_node; //set previous node to current node
            curr_node = next_node; //set current node to next node

        } else { //since next code is NULL
            write_pair(outfile, curr_node->code, curr_sym, bit_length(next_code)); //write pair
            curr_node->children[curr_sym] = trie_node_create(next_code); //set current node
            curr_node = tr; //set current node to trie root
            next_code += 1; //increment next code by 1
        }
        if (next_code == MAX_CODE) { //if next code equals max code
            trie_reset(tr); //reset trie root
            curr_node = tr; //current node equals trie root
            next_code = START_CODE; //next code equals start code
        }
        prev_sym = curr_sym; //previous symbol equals current symbol
    }
    if (curr_node != tr) { //if current node doesn't equal trie root
        write_pair(
            outfile, prev_node->code, prev_sym, bit_length(next_code)); //write pair to outfile
        next_code = (next_code + 1) % MAX_CODE; //set next code
    }
    write_pair(outfile, STOP_CODE, 0, bit_length(next_code)); //write pair to outfile
    flush_pairs(outfile); //flush pairs

    if (print_stats == true) { //if print stats
        uint64_t cb = (total_bits); //calculates compressed bytes
        uint64_t ub = (total_syms); //calculate uncompressed bytes
        double ratio = 100 * (1 - ((double) cb / ub)); //calculates ratio

        printf("Compressed file size: %lu bytes\n", cb); //print compressed file size
        printf("Uncompressed file size: %lu bytes\n", ub); //print uncompressed file size
        printf("Compression ratio: %.2f%%\n", ratio); //print compression ratio
    }

    close(infile); //close infile
    close(outfile); //close outfile

    trie_delete(tr); //delete trie

    return 0;
}
