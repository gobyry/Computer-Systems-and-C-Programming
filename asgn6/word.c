#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "word.h"
#include "code.h"

Word *word_create(uint8_t *syms, uint32_t len) { //constructes word
    Word *w = (Word *) malloc(sizeof(Word)); //allocate word memory
    w->syms = (uint8_t *) malloc(len * sizeof(uint8_t)); //create symbol array

    if ((w != NULL) && (w->syms != NULL)) { //if successful
        w->len = len; //set length
        for (uint8_t i = 0; i < len; i += 1) { //loop through length
            w->syms[i] = syms[i]; //copy ith symbol of symbol array
        }
        return w; //return word

    } else { //null otherwise
        free(w->syms); //free symbols memory
        free(w); //free word memory
        return NULL;
    }
}

Word *word_append_sym(Word *w, uint8_t sym) { //constructs new word
    uint32_t len = w->len + 1; //set length value
    uint8_t *new_syms = (uint8_t *) malloc(len * sizeof(uint8_t)); //constructs new symbol
    Word *new_word = (Word *) malloc(sizeof(Word)); //constructs new word

    if (w == NULL || new_syms == NULL || new_word == NULL) { //if word is empty
        free(new_syms); //free memory
        free(new_word); //free memory
        return NULL;
    } else { //if successful
        for (uint8_t i = 0; i < (len - 1); i += 1) { //loops through words
            new_syms[i] = w->syms[i]; //appends new symbol
        }
    }
    new_syms[len - 1] = sym; //set new symbols
    new_word->syms = new_syms; //set new word symbols
    new_word->len = len; //set new word length

    return new_word; //return new appended word
}

void word_delete(Word *w) { //destroys word
    free(w->syms); //free symbols
    (w->syms) = NULL; //set symbols to null
    free(w); //free word
    w = NULL; //set word to null
}

WordTable *wt_create(void) { //creates new word table
    WordTable *wt = (WordTable *) malloc(sizeof(Word *) * MAX_CODE); //allocate word table memory
    if (wt == NULL) { //if unsucessful
        return NULL;
    }
    wt[EMPTY_CODE] = word_create(NULL, 0); //set word table index to empty code
    return wt; //return word table
}

void wt_reset(WordTable *wt) { //resets wordtable
    if (wt == NULL) { //if unsucessful
        return;
    }
    for (int i = 0; i < MAX_CODE; i += 1) { //loop through word table
        if (wt[i] != NULL && i >= START_CODE) { //if word isn't null
            word_delete(wt[i]); //delete word
            wt[i] = NULL; //set word to null
        }
    }
}

void wt_delete(WordTable *wt) { //deletes wordtable
    if (wt == NULL) { //if unsucessful
        return;
    }
    wt_reset(wt); //call wt reset on word table
    free(wt); //free word table
}
