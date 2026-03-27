/**
 * @file dynamicstack.c
 * @brief Implementation of the DynamicStack Abstract Data Type.
 *
 * The stack is maintained as a singly linked list where the head of the list
 * is always the top of the stack. This gives O(1) push, pop, and peek without
 * ever needing to shift elements.
 *
 * Memory layout after three pushes (1, then 2, then 3):
 *
 *   s.top ──► [ data=3 | next ]──► [ data=2 | next ]──► [ data=1 | next=NULL ]
 *               (topo)                                     (base)
 *
 * Push: allocate a new node, point its next at the current top, update top.
 * Pop : save top's data, advance top to top->next, free the old top node.
 */

#include <stdio.h>
#include "dynamicstack.h"

void ds_init(DynamicStack *s) {
    (*s).top  = NULL;
    (*s).size = 0;
}

int ds_push(DynamicStack *s, int elem) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return 0; /* allocation failure — caller must handle */
    }

    (*node).data = elem;
    (*node).next = (*s).top; /* new node points down to old top */
    (*s).top     = node;     /* stack top is now the new node   */
    (*s).size++;
    return 1;
}

int ds_pop(DynamicStack *s, int *out) {
    if (ds_isEmpty(s)) {
        return 0; /* underflow — caller must handle */
    }

    Node *removed = (*s).top;
    *out          = (*removed).data;
    (*s).top      = (*removed).next; /* expose the node below */
    free(removed);
    (*s).size--;
    return 1;
}

int ds_peek(const DynamicStack *s, int *out) {
    if (ds_isEmpty(s)) {
        return 0;
    }

    *out = (*(*s).top).data;
    return 1;
}

int ds_isEmpty(const DynamicStack *s) {
    return (*s).top == NULL;
}

int ds_size(const DynamicStack *s) {
    return (*s).size;
}

void ds_print(const DynamicStack *s) {
    if (ds_isEmpty(s)) {
        printf("(pilha vazia)\n");
        return;
    }

    printf("Topo\n");
    printf("----\n");

    /* walk the linked list from top (head) toward base (tail) */
    Node *current = (*s).top;
    int   pos     = (*s).size - 1; /* display index: top element gets highest index */
    while (current != NULL) {
        printf("[%2d] %d\n", pos, (*current).data);
        current = (*current).next;
        pos--;
    }

    printf("----\n");
    printf("Base\n");
}

void ds_free(DynamicStack *s) {
    int discarded;
    /* reuse ds_pop so each node is properly freed */
    while (ds_pop(s, &discarded)) {
        /* nothing extra needed — ds_pop frees the node */
    }
}
