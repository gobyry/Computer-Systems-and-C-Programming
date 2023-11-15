#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"
#include "code.h"

TrieNode *trie_node_create(uint16_t index) { //constructes trie node
    TrieNode *tn = (TrieNode *) malloc(sizeof(TrieNode)); //create trie nodes

    if (tn) { //if successful
        tn->code = index; //set trie node code to code
        for (uint16_t i = 0; i < ALPHABET; i += 1) { //loop through alphabet
            tn->children[i] = NULL; //set each child to null
        }
    } else {
        return NULL;
    }
    return tn; //return trie node
    free(tn); //free trie node memory
}

void trie_node_delete(TrieNode *n) { //destroys trie node
    free(n); //free node
    n = NULL; //set node to null
}

TrieNode *trie_create(void) { //intializes trie
    TrieNode *tr = trie_node_create(EMPTY_CODE); //create trie root
    if (tr) { //if successful
        return tr; //return trie root
    } else {
        return NULL; //return null if not sucessful
    }
}

void trie_reset(TrieNode *root) { //resets trie
    for (uint16_t i = 0; i < ALPHABET; i += 1) { //loops through alphabet
        if (root->children[i]) { //if child exists
            trie_node_delete(root->children[i]); //deletes child
            root->children[i] = NULL; //sets child in root to null
        }
    }
}

void trie_delete(TrieNode *n) { //deletes sub-trie
    if (n) { //if successful
        for (uint16_t i = 0; i < ALPHABET; i += 1) { //loops throughs alphabet
            if (n->children[i]) { //if child exists
                trie_delete(n->children[i]); //recursively deletes child
                n->children[i] = NULL; //sets child to null
            }
        }
        trie_node_delete(n); //delete node
    }
}

TrieNode *trie_step(TrieNode *n, uint8_t sym) { //returns a pointer to child
    if (!sym) { //if symbol doesn't exist
        return NULL;
    } else {
        return n->children[sym]; //else return children symbol
    }
}
