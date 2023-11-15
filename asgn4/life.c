#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:h"

void message(void) { //print program usage
    fprintf(stderr, "SYNOPSIS\n"
                    "    Conway's Game of Life\n"
                    "\n"
                    "USAGE\n"
                    "    ./life tsn:i:o:h\n"
                    "\n"
                    "OPTIONS\n"
                    "    -t             Create your universe as a toroidal\n"
                    "    -s             Silent - do not use animate the evolution\n"
                    "    -n {number}    Number of generations [default: 100]\n"
                    "    -i {file}      Input file [default: stdin]\n"
                    "    -o {file}      Output file [default: stdout]\n");
}

void uni_swap(Universe **uni_a, Universe **uni_b) { //swap universe function
    Universe *uni_temp = *uni_a; //create temp universe
    *uni_a = *uni_b; //swap a with b
    *uni_b = uni_temp; //swap b with temp
}

int main(int argc, char **argv) {
    int opt = 0;
    bool dfault = true;

    //defaults
    bool tor = false;
    bool sil = false;
    int gen = 100;
    uint32_t row = 0;
    uint32_t col = 0;

    FILE *infile; //infile
    FILE *outfile; //outfile

    char *in = NULL; //stdin
    char *out = NULL; //stdout

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': //specify for toroidal
            tor = true;
            dfault = false;
            break;
        case 's': //silence ncurses
            sil = true;
            dfault = false;
            break;
        case 'n': //specify generations
            gen = atoi(optarg);
            if (1 <= gen && gen <= 10000000) { //error handling for generations
                continue;
            } else {
                printf("%d is an invalid number of generations.\n", gen); //error user input
                return 1;
                break;
            }
            dfault = false;
            break;
        case 'i': //specify infile
            in = optarg;
            dfault = false;
            break;
        case 'o': //specify outfile
            out = optarg;
            dfault = false;
            break;
        case 'h': //program usage
            dfault = true;
            break;
        }
    }
    if (dfault == true) { //if h is true or no command input
        message();
        return 1;
    }
    infile = in != NULL ? fopen(in, "r") : stdin; //open infile
    if (!infile) { //infile error message
        printf("Error opening %s.\n", in); //return file name that errored
        return 1;
    }

    outfile = out != NULL ? fopen(out, "w") : stdout; //open outfile
    if (!outfile || !infile) { //outfile error message
        printf("Error opening %s.\n", out); //return file name that errored
        return 1;
    }

    int num = (fscanf(infile, "%u %u\n", &row, &col)); //perform initial fscanf
    if (num < 2) { //error handling for rows and cols
        printf("The number of rows and columns are invalid.\n");
        return 1;
    }

    Universe *uni_a = uv_create(row, col, tor); //create universe a
    Universe *uni_b = uv_create(row, col, tor); //create universe b

    uv_populate(uni_a, infile); //populate universe a

    for (int i = 0; i < gen; i += 1) { //loops through gen
        if (sil == false) { //if ncurses enabled
            initscr(); //initialize screen
            curs_set(FALSE); //hide cursor
            clear(); //clear screen
        }
        if (uv_rows(uni_a) < 1 || uv_cols(uni_a) < 1) { //error handling for rows and cols
            printf("The numbers of rows and columns are invalid.\n");
            return 1;
        }
        for (uint32_t row = 0; row < uv_rows(uni_a); row += 1) { //loop through rows
            for (uint32_t col = 0; col < uv_cols(uni_a); col += 1) { //loop through cols

                int neighbor = uv_census(uni_a, row, col); //neighbors
                bool alive = uv_get_cell(uni_a, row, col); //alive

                if (alive && (neighbor == 2 || neighbor == 3)) { //rule 1 = alive
                    uv_live_cell(uni_b, row, col); //set live cell for universe b
                } else if (!alive && neighbor == 3) { //rule 2 = alive
                    uv_live_cell(uni_b, row, col); //set dead cell for universe b
                } else { //rule 3, else dead
                    uv_dead_cell(uni_b, row, col); //set dead cell
                }
                if (sil == false) {
                    if (uv_get_cell(uni_b, row, col)) { //if cell is alive
                        mvprintw(row, col, "o"); //prints alive cell
                    }
                }
            }
        }
        if (sil == false) {
            refresh(); //refresh screen
            usleep(50000); //sleep for 50k microseconds
        }
        uni_swap(&uni_a, &uni_b); //swaps universe a with universe b
    }

    if (sil == false) {
        endwin(); //closes window
    }

    uv_print(uni_a, outfile); //prints universe to outfile

    uv_delete(uni_a); //delete uni a
    uv_delete(uni_b); //delete uni b
    
    fclose(infile); //close infile
    fclose(outfile); //close outfile
    return 0;
}
