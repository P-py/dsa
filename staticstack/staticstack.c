/**
 * @file staticstack.c
 * @brief Implementation of the StaticStack Abstract Data Type.
 *
 * Internally the stack is an integer array (items) paired with a "top" cursor.
 * The cursor holds the index of the most recently pushed element:
 *
 *   top == -1           → stack is empty
 *   top == STACKSIZE-1  → stack is full
 *
 * Push: increment top, then write the new value at items[top].
 * Pop : read items[top], then decrement top.
 *
 * No dynamic memory is used; all storage lives inside the StaticStack struct.
 */

#include <stdio.h>
#include "staticstack.h"

void st_init(StaticStack *s) {
    (*s).top = -1; /* -1 is the sentinel that means "no elements yet" */
}

int st_push(StaticStack *s, int elem) {
    if (st_isFull(s)) {
        return 0; /* overflow — caller must handle */
    }

    (*s).top++;
    (*s).items[(*s).top] = elem;
    return 1;
}

int st_pop(StaticStack *s, int *out) {
    if (st_isEmpty(s)) {
        return 0; /* underflow — caller must handle */
    }

    *out = (*s).items[(*s).top];
    (*s).top--;
    return 1;
}

int st_peek(const StaticStack *s, int *out) {
    if (st_isEmpty(s)) {
        return 0;
    }

    *out = (*s).items[(*s).top];
    return 1;
}

int st_isEmpty(const StaticStack *s) {
    return (*s).top == -1;
}

int st_isFull(const StaticStack *s) {
    return (*s).top == STACKSIZE - 1;
}

int st_size(const StaticStack *s) {
    /* top is a 0-based index, so element count is top + 1 */
    return (*s).top + 1;
}

void st_print(const StaticStack *s) {
    if (st_isEmpty(s)) {
        printf("(pilha vazia)\n");
        return;
    }

    printf("Topo\n");
    printf("----\n");
    /* iterate from top down to the bottom so printout matches LIFO order */
    for (int i = (*s).top; i >= 0; i--) {
        printf("[%2d] %d\n", i, (*s).items[i]);
    }
    printf("----\n");
    printf("Base\n");
}
