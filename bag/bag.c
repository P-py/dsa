#include "bag.h"
#include <stdio.h>

/*
* typedef struct b {
*    int item[BAGSIZE];
* } Bag;
*/

void init(Bag *b) {
    for (int i=0; i<BAGSIZE; i++) {
        (*b).item[i] = -1;
    }
}

int size(Bag *b) {
    int count = 0;

    for (int i=0; i<BAGSIZE; i++) {
        if ((*b).item[i] > -1) {
            printf("\n%d", (*b).item[i]);
            count++;
        } 
    }

    return count;
}

int insert(Bag *b, int elem) {
    for (int i=0; i<BAGSIZE; i++) {
        if ((*b).item[i] == -1) {
            (*b).item[i] = elem;
            return 1;
        }
    }

    return 0;
}

int check(Bag *b, int elem) {
    int count = 0;
    
    for (int i=0; i<BAGSIZE; i++) {
        if ((*b).item[i] == elem) {
            count++;
        }
    }

    return count;
}

int delete(Bag *b, int elem) {
    for (int i=0; i<BAGSIZE; i++) {
        if ((*b).item[i] == elem) {
            (*b).item[i] = -1;
            return 1;
        }
    }
    
    return 0;
}

void printall(Bag *b) {
    for (int i=0; i<BAGSIZE; i++) {
        printf("%d\n", (*b).item[i]);
    }
}
