#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "trie.h"
#include "word.h"
#include "code.h"
#include "endian.h"
#include "io.h"

struct FileHeader;

//externed variables
uint64_t total_syms;
uint64_t total_bits;

//global variables
uint8_t pair_buffer[BLOCK];
uint8_t word_buffer[BLOCK];
int pair_len = 0;
int word_len = 0;

int read_bytes(int infile, uint8_t *buf, int to_read) { //perform reads
    int total = 0; //total bytes read
    int br = 0; //bytes read

    while (total < to_read) { //loop through file
        br = read(infile, buf + total, to_read - total);
        if (br == -1) { //bytes error
            return 1;
        }
        if (br == 0) { //no more bytes
            break;
        } else {
            total += br; //add bytes read to total bytes read
        }
    }
    total_syms += total;
    return total; //return total bytes read
}

int write_bytes(int outfile, uint8_t *buf, int to_write) { //perform writes
    int total = 0; //total bytes wrtten
    int bw = 0; //bytes written

    while (total < to_write) { //loop through file
        bw = write(outfile, buf + total, to_write - total); //
        if (bw == 0) { //no more bytes
            break;
        }
        total += bw; //add bytes written to total bytes wrttien
    }
    total_bits += total;
    return total; //return total bytes written
}

void read_header(int infile, FileHeader *header) { //reads in sizeof bytes
    uint8_t br = read_bytes(infile, (void *) header, sizeof(FileHeader)); //set bytes read

    if (br != sizeof(FileHeader)) { //if unsuccessful
        printf("File header read failure.\n");
        return;
    }
    if (header->magic != MAGIC) { //if magic is successful
        header = 0;
        printf("Invalid file format.\n");
        return;
    }
    if (big_endian()) { //check for endianess
        header->magic = swap32(header->magic); //swap magic
        header->protection = swap16(header->protection); //swap protection
    }
}

void write_header(int outfile, FileHeader *header) { //writes in sizeof bytes
    if (big_endian()) { //if not little endian
        header->magic = swap32(header->magic); //swap endianness of magic
        header->protection = swap16(header->protection); //swap endianness of protection
    }
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader)); //write header to output file
}

bool read_sym(int infile, uint8_t *sym) { //keeps track of read symbol
    static uint8_t buffer[BLOCK]; //set buffer
    static int len = 0; //set length of buffer
    static int pos = 0; //set buffer position

    if (len <= pos) { //if successful, read in new block
        len = read_bytes(infile, buffer, BLOCK); //length equal read bytes
        pos = 0; //reset position

        if (len == 0) { //if not full block and EOF
            return false;
        }
    }
    *sym = buffer[pos]; //read in next symbol from buffer
    pos += 1; //increment position by 1

    return true;
}

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) { //writes pair to outfile
    for (int i = 0; i < bitlen; i += 1) { //loop through bits
        if (code & (1 << i)) { //if ith bit is 1
            pair_buffer[pair_len / 8] |= (1 << (pair_len % 8)); //set ith bit to 1
        } else { //if ith bit is 0
            pair_buffer[pair_len / 8] &= ~(1 << (pair_len % 8)); //set ith bit to 0
        }
        pair_len += 1; //increment length by 1

        if (pair_len == BLOCK * 8) { //if buffer is full
            write_bytes(outfile, pair_buffer, BLOCK); //write out bytes to outfile
        }
    }

    for (int i = 0; i < 8; i += 1) { //loop through bits
        if (sym & (1 << i)) { //if ith bit is 1
            pair_buffer[pair_len / 8] |= (1 << (pair_len % 8)); //set ith bit to 1
        } else { //if ith bit is 0
            pair_buffer[pair_len / 8] &= ~(1 << (pair_len % 8)); //set ith bit to 0
        }
        pair_len += 1; //increment buffer by 1

        if (pair_len == BLOCK * 8) { //if buffer is full
            write_bytes(outfile, pair_buffer, BLOCK); //write out bytes to outfile
        }
    }
}

void flush_pairs(int outfile) { //writes remaining symbols to outfile
    if (pair_len > 0) { //if there are remaining bytes
        write_bytes(outfile, pair_buffer, (pair_len + 7) / 8); //flush to outfile
    }
    pair_len = 0; //reset buffer length
}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) { //reads pair from infile
    static uint8_t buffer[BLOCK]; //buffer with block
    static int bits = 0; //set bits
    static int read = 0; //set bytes read
    *code = 0; //set code
    *sym = 0; //set sym

    if (bits == 0) { //if no bits, read data
        read = read_bytes(infile, buffer, BLOCK); //read bytes from infile
        if (read == 0) { //if unsucessful
            return false;
        }
    }

    for (int i = 0; i < bitlen; i += 1) { //loop through code bits
        *code |= ((buffer[bits / 8] >> (bits % 8)) & 1) << i; //set code from buffer
        bits += 1; //increment length by 1
    }

    for (int i = 0; i < 8; i += 1) { //loop through symbol bits
        *sym |= ((buffer[bits / 8] >> (bits % 8)) & 1) << i; //set sym from buffer
        bits += 1;
    }

    if (bits >= read * 8) { //if bits are to large
        bits = 0; //reset bits
        read = 0; //reset read bytes
    }
    return (*code != STOP_CODE); //return read code is not stop code
}

void write_word(int outfile, Word *w) { //writes to outfile
    for (uint8_t i = 0; i < w->len; i += 1) { //loop through symbols
        word_buffer[word_len] = w->syms[i]; //add symbol to buffer
        word_len += 1; //increment length
        if (word_len == BLOCK) { //if buffer full
            write_bytes(outfile, word_buffer, word_len); //writes bytes to outfile
            word_len = 0; //reset length
        }
    }
}

void flush_words(int outfile) { //writes remaining symbols to outfile
    if (word_len > 0) { //if there are remaining bytes
        write_bytes(outfile, word_buffer, word_len); //flush to outfile
    }
    word_len = 0; //reset buffer length
}
