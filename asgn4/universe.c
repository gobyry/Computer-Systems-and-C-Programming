#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Universe { //universe structure
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) { //constructor for universe
    Universe *u = calloc(1, sizeof(*u)); //allocate memory for u

    u->rows = rows; //accesses rows through u
    u->cols = cols; //accesses cols
    u->toroidal = toroidal; //accesses toroidal

    //example constructor from pdf
    u->grid = (bool **) calloc(rows, sizeof(bool *)); //allocate memory for rows
    for (uint32_t r = 0; r < rows; r += 1) { //loop through rows
        u->grid[r] = (bool *) calloc(cols, sizeof(bool)); //allocate memory for cols
    }
    //u->grid = grid; //assigns grid memory to u
    return u; //returns constructed universe
}

void uv_delete(Universe *u) { //frees memory allocated
    uint32_t rows = u->rows;
    for (uint32_t r = 0; r < rows; r += 1) { //loops through rows
        free(u->grid[r]); //frees every grid row
    }
    free(u->grid); //frees grid
    free(u); //frees universe
}

uint32_t uv_rows(Universe *u) { //return universe rows
    uint32_t rows = u->rows;
    return (rows);
}

uint32_t uv_cols(Universe *u) { //return universe columns
    uint32_t cols = u->cols;
    return (cols);
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) { //manipulates if cell is alive
    if (r >= 0 && r < u->rows && c >= 0 && c < u->cols) { //if cell is in bounds
        u->grid[r][c] = true; //cell is declared alive
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) { //manipulates if cell is dead
    if (r >= 0 && r < u->rows && c >= 0 && c < u->cols) { //if cell is in bounds
        u->grid[r][c] = false; //cell is declared dead
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) { //returns cell if alive
    if (r < 0 || r >= u->rows || c < 0
        || c >= u->cols) { //negative or greater than grid equals out ouf bounds
        return false;
    }
    return u->grid[r][c]; //returns grid cell if alive
}

bool uv_out_bounds(Universe *u, uint32_t r, uint32_t c) {
    if (r < 0 || r >= u->rows || c < 0
        || c >= u->cols) { //negative or greater than grid equals out ouf bounds
        return true;
    }
    return false;
}

bool uv_populate(Universe *u, FILE *infile) { //populate universe with row and column pairs
    uint32_t row = 0;
    uint32_t col = 0;
    while (
        fscanf(infile, "%u %u\n", &row, &col) != EOF) { //loop through to obtain row and col values
        if (uv_out_bounds(u, row, col)) { //set cell dead
            return false;
        }
        uv_live_cell(u, row, col); //set cell alive
    }
    return true;
}

uint32_t uv_census(
    Universe *u, uint32_t r, uint32_t c) { //returns # of live neighbors adjacent to row r and col c
    uint32_t neighbors = 0;
    bool tor = u->toroidal;
    for (int x = -1; x < 2; x += 1) { //making x values -1,0,1
        for (int y = -1; y < 2; y += 1) { //making y values -1,0,1
            if (x == 0 && y == 0) { //x y = (0,0)
                continue;
            }
            if (!tor) {
                if (uv_out_bounds(u, r + x, c + y)) { //checks if out of bounds for the 3x3 area
                    continue;
                } else if (uv_get_cell(u, r + x,
                               c + y)) { //checks if neighbors are in bound for the 3x3 area
                    neighbors += 1; //incremenet neighbor
                }
            }
            if (tor) {
                if (uv_out_bounds(u, (r + x + u->rows) % u->rows,
                        (c + y + u->cols) % u->cols)) { //checks if out of bounds for the 3x3 area
                    continue;
                } else if (uv_get_cell(u, (r + x + u->rows) % u->rows,
                               (c + y + u->cols)
                                   % u->cols)) { //checks if neighbors are in bound for the 3x3 area
                    neighbors += 1;
                }
            }
        }
    }
    return neighbors; //# of live neighbors
}

void uv_print(Universe *u, FILE *outfile) { //prints universe to outfile
    uint32_t rows = u->rows;
    uint32_t cols = u->cols;
    for (uint32_t r = 0; r < rows; r += 1) { //loops through rows
        for (uint32_t c = 0; c < cols; c += 1) { //loops through cols
            if (uv_get_cell(u, r, c)) {
                fprintf(outfile, "o"); //if alive prind o (alive)
            } else {
                fprintf(outfile, "."); //if dead print . (dead)
            }
        }
        fprintf(outfile, "\n"); //prints new line to outfile
    }
}
