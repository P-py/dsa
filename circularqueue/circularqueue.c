/**
 * @file circularqueue.c
 * @brief Implementation of the CircularQueue Abstract Data Type.
 *
 * Internally the queue is an integer array (data) paired with two cursors and
 * an element count:
 *
 *   count == 0         → queue is empty
 *   count == QUEUESIZE → queue is full
 *
 * Both head and tail advance modulo QUEUESIZE, forming a ring:
 *
 *   Enqueue: write elem at data[tail], then tail = (tail + 1) % QUEUESIZE.
 *   Dequeue: read data[head], then head = (head + 1) % QUEUESIZE.
 *
 * Using a separate count field lets us distinguish the full case (head == tail,
 * count == QUEUESIZE) from the empty case (head == tail, count == 0) without
 * sacrificing a slot in the array.
 *
 * No dynamic memory is used; all storage lives inside the CircularQueue struct.
 */

#include <stdio.h>
#include "circularqueue.h"

void cq_init(CircularQueue *q) {
    (*q).head = 0;
    (*q).tail = -1;
    (*q).count = 0;
}

void cq_destroy(CircularQueue *q) {
    /* Static queue: no heap memory to release; just reset to a safe state. */
    cq_init(q);
}

int cq_enqueue(CircularQueue *q, int elem) {
    if (cq_isFull(q)) {
        return 0; /* overflow — caller must handle */
    }

    (*q).tail = ((*q).tail + 1) % QUEUESIZE;
    (*q).data[(*q).tail] = elem;
    (*q).count++;
    return 1;
}

int cq_dequeue(CircularQueue *q, int *out) {
    if (cq_isEmpty(q)) {
        return 0; /* underflow — caller must handle */
    }

    *out = (*q).data[(*q).head];
    (*q).head = ((*q).head + 1) % QUEUESIZE;
    (*q).count--;
    return 1;
}

int cq_peek(const CircularQueue *q, int *out) {
    if (cq_isEmpty(q)) {
        return -1;
    }

    *out = (*q).data[(*q).head];
    return 1;
}

int cq_isEmpty(const CircularQueue *q) {
    return (*q).count == 0;
}

int cq_isFull(const CircularQueue *q) {
    return (*q).count == QUEUESIZE;
}

int cq_size(const CircularQueue *q) {
    return (*q).count;
}
