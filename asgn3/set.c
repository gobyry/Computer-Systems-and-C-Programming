#include "set.h"

#include <stdio.h>

Set set_empty(void) { //returns an empty set
    return 0;
}

Set set_universal(void) { //returns every element in the set
    return ~0;
}

Set set_insert(Set s, uint8_t x) { //inserts x into s, returns x bit set to 1
    return s | (1 << (x % 32));
}

Set set_remove(Set s, uint8_t x) { //deletes x from s, returns s with a bit equal to x set to 0
    return s & ~(1 << (x % 32));
}

bool set_member(Set s, uint8_t x) { //if bit is in set
    return s & (1 << (x % 32));
}

Set set_union(Set s, Set t) { //union of two sets
    return s | t;
}

Set set_intersect(Set s, Set t) { //intersection of two sets
    return s & t;
}

Set set_difference(Set s, Set t) { //elements in s not in t
    return s & ~t;
}
