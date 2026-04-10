/**
 * @file   problem2.c
 * @brief  Simplified LRU (Least Recently Used) Virtual Memory Manager.
 *
 * Models a RAM of fixed capacity N as a singly-linked list of Pages.
 * When RAM is full and a new page must be loaded, the page that was used
 * least recently (lowest timestamp) is evicted and the new page is prepended.
 *
 * Data structure chosen: singly-linked list.
 *   - Pages are accessed/evicted one at a time, so O(n) search is acceptable.
 *   - Insertion at the head is O(1) — no capacity-bounded array shifting.
 *   - Removal of an arbitrary node only requires a pointer re-link, making it
 *     O(1) once the target is found.
 *
 * Scalability note: with 1 million pages the eviction scan is still O(n),
 * meaning every replacement traverses the entire list. This is acceptable for
 * a prototype but a production LRU cache would couple the list with a min-heap
 * or sorted structure to bring eviction down to O(log n).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------------
 * Data type
 * -------------------------------------------------------------------------*/

/**
 * @brief A single memory page held in the simulated RAM.
 *
 * @param id        Unique identifier of the page (assigned by the OS).
 * @param timestamp Last-access time. Lower values mean the page was used
 *                  longer ago and is therefore the best eviction candidate.
 * @param next      Intrusive pointer linking this page to the next one in the
 *                  RAM list.  NULL marks the end of the list.
 */
typedef struct Page {
    int id;
    int timestamp;
    struct Page *next;
} Page;

/* ---------------------------------------------------------------------------
 * Internal helpers
 * -------------------------------------------------------------------------*/

/**
 * @brief  Allocates and initialises a new Page on the heap.
 *
 * @param  id         Page identifier.
 * @param  timestamp  Last-access timestamp for LRU ordering.
 * @return Pointer to the newly allocated Page.
 *
 * @note   Caller is responsible for eventually freeing the returned node.
 */
Page *createPage(int id, int timestamp) {
    /* Reserve exactly one Page worth of heap memory. */
    Page *p = (Page *)malloc(sizeof(Page));

    (*p).id        = id;
    (*p).timestamp = timestamp;
    (*p).next      = NULL;   /* isolated node — no successor yet */

    return p;
}

/**
 * @brief  Prepends a new page to the linked list.
 *
 * Inserts at the head in O(1):
 *   1. Create the new node.
 *   2. Make the new node point to the current head.
 *   3. Advance the head pointer to the new node.
 *
 * @param head       Address of the caller's head pointer (Page **) so the
 *                   head can be updated in place.
 * @param id         Identifier for the new page.
 * @param timestamp  Access timestamp for the new page.
 */
void insertAtBeginning(Page **head, int id, int timestamp) {
    Page *p = createPage(id, timestamp);

    /* Link the new node in front of whatever is currently at the head. */
    (*p).next = *head;

    /* The caller's head pointer now points to the new node. */
    *head = p;
}

/* ---------------------------------------------------------------------------
 * Core LRU replacement
 * -------------------------------------------------------------------------*/

/**
 * @brief  Evicts the least recently used page and loads a new one.
 *
 * Algorithm (single O(n) pass):
 *   - Walk the list tracking `lru` (node with the current minimum timestamp)
 *     and `lruPrev` (its immediate predecessor, needed for unlinking).
 *   - After the walk, unlink `lru` from the list and release its memory.
 *   - Prepend the new page so it is immediately reachable at the head.
 *
 * Why track the predecessor?
 *   A singly-linked list has no back-pointer. To remove a node we must
 *   redirect its predecessor's `next` to skip over it. Without `lruPrev` we
 *   would need a second traversal to locate the predecessor.
 *
 * @param head          Address of the caller's head pointer.
 * @param newId         Identifier of the incoming page.
 * @param newTimestamp  Access timestamp of the incoming page.
 */
void replaceLruPage(Page **head, int newId, int newTimestamp) {
    /* Nothing to evict from an empty list. */
    if (*head == NULL) return;

    /*
     * Initialise the search with the head node as the first LRU candidate.
     * `prev` trails one step behind `curr` during traversal so we can record
     * `lruPrev` the moment a new minimum is found.
     *
     *   lruPrev ──► [lru] ──► ...
     *   prev    ──► [curr] ──► ...
     */
    Page *prev    = NULL;   /* node just before curr in the traversal    */
    Page *lruPrev = NULL;   /* predecessor of the current LRU candidate  */
    Page *lru     = *head;  /* current LRU candidate (lowest timestamp)  */

    /* Start curr one step ahead of lru to begin comparing from node 2. */
    Page *curr = (**head).next;

    while (curr != NULL) {
        /* If this node was accessed even less recently, update the candidate. */
        if ((*curr).timestamp < (*lru).timestamp) {
            lru     = curr;   /* new eviction target     */
            lruPrev = prev;   /* remember its predecessor */
        }

        /* Advance both trailing pointers one step forward. */
        prev = curr;
        curr = (*curr).next;
    }

    /*
     * Unlink `lru` from the list.
     *
     * Case A — lru is the head (lruPrev == NULL):
     *   The head pointer itself must advance past lru.
     *
     *   Before:  head ──► [lru] ──► [X] ──► ...
     *   After:   head ──────────────► [X] ──► ...
     *
     * Case B — lru is an interior or tail node:
     *   Redirect the predecessor's next pointer to skip over lru.
     *
     *   Before:  [lruPrev] ──► [lru] ──► [X] ──► ...
     *   After:   [lruPrev] ─────────────► [X] ──► ...
     */
    if (lruPrev == NULL) {
        *head = (*lru).next;
    } else {
        (*lruPrev).next = (*lru).next;
    }

    /* Release the evicted page's heap memory. */
    free(lru);

    /* Prepend the newly loaded page so it sits at the front of the list. */
    insertAtBeginning(head, newId, newTimestamp);
}

/* ---------------------------------------------------------------------------
 * Utilities
 * -------------------------------------------------------------------------*/

/**
 * @brief  Prints every page currently in the RAM list.
 *
 * Output format:  RAM: [id=X ts=Y] -> [id=X ts=Y] -> ...
 *
 * @param head  Head of the list (passed by value; the original is unmodified).
 */
void printList(Page *head) {
    printf("RAM: ");
    while (head != NULL) {
        printf("[id=%d ts=%d]", (*head).id, (*head).timestamp);

        /* Print the arrow connector only when a successor exists. */
        if ((*head).next != NULL) printf(" -> ");

        head = (*head).next;
    }
    printf("\n");
}

/**
 * @brief  Releases every node in the list, preventing memory leaks.
 *
 * The node pointer is saved in `tmp` before `head` advances, because freeing
 * `head` directly would make reading `head->next` undefined behaviour.
 *
 * @param head  Head of the list (passed by value).
 */
void freeList(Page *head) {
    while (head != NULL) {
        Page *tmp = head;          /* save current node before moving on */
        head      = (*head).next; /* advance before the node is freed   */
        free(tmp);
    }
}

/* ---------------------------------------------------------------------------
 * Entry point
 * -------------------------------------------------------------------------*/

/**
 * @brief  Demonstrates the LRU replacement policy with a 4-page RAM.
 *
 * Initial state (timestamps in parentheses):
 *   id=4(1) -> id=3(7) -> id=2(3) -> id=1(10)
 *
 * Replacement 1 — load id=5 (ts=15):
 *   Evicts id=4 (ts=1, the global minimum).
 *   Result: id=5(15) -> id=3(7) -> id=2(3) -> id=1(10)
 *
 * Replacement 2 — load id=6 (ts=20):
 *   Evicts id=2 (ts=3, now the global minimum).
 *   Result: id=6(20) -> id=5(15) -> id=3(7) -> id=1(10)
 */
int main(void) {
    /* NULL-initialised head represents an empty RAM. */
    Page *ram     = NULL;
    int   capacity = 4;

    /* Fill RAM to capacity using sequential insertions at the head. */
    insertAtBeginning(&ram, 1, 10);
    insertAtBeginning(&ram, 2, 3);
    insertAtBeginning(&ram, 3, 7);
    insertAtBeginning(&ram, 4, 1);
    printList(ram);

    /* RAM is full — trigger the first LRU eviction. */
    printf("RAM full (%d pages). Loading page id=5 (ts=15), evicting LRU...\n", capacity);
    replaceLruPage(&ram, 5, 15);
    printList(ram);

    /* Trigger a second eviction to confirm the policy holds after mutation. */
    printf("Loading page id=6 (ts=20), evicting LRU...\n");
    replaceLruPage(&ram, 6, 20);
    printList(ram);

    /* Release all remaining heap-allocated nodes before exit. */
    freeList(ram);
    return 0;
}
