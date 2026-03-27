/**
 * @file dynamicstack.h
 * @brief Interface for the DynamicStack Abstract Data Type.
 *
 * A DynamicStack is a LIFO (Last-In, First-Out) collection backed by a singly
 * linked list. Unlike StaticStack it has no fixed capacity — it grows and shrinks
 * at runtime through heap allocation. The only hard limit is available memory.
 *
 * Each element is wrapped in a Node that holds the value and a pointer to the
 * node below it. The "top" pointer of DynamicStack always points to the most
 * recently pushed node (or NULL when the stack is empty).
 *
 * Because nodes are heap-allocated, ds_free() MUST be called when the stack is
 * no longer needed to avoid memory leaks.
 *
 * All mutating operations report success (1) or failure (0) as their return value.
 */

#include <stdlib.h>

/**
 * @brief A single node in the linked list that backs the DynamicStack.
 *
 * Fields:
 *   data - the integer value stored in this node.
 *   next - pointer to the node immediately below this one, or NULL at the base.
 */
typedef struct Node {
    int data;
    struct Node *next;
} Node;

/**
 * @brief Represents an unbounded LIFO stack of integers.
 *
 * @note Do not access or modify the fields directly; use the provided functions.
 *
 * Fields:
 *   top  - pointer to the topmost node; NULL when the stack is empty.
 *   size - number of nodes currently allocated (kept in sync by push/pop).
 */
typedef struct {
    Node *top;
    int   size;
} DynamicStack;

/**
 * @brief Initialises a DynamicStack, making it ready for use.
 * @param s Pointer to the DynamicStack to initialise. Must not be NULL.
 */
void ds_init(DynamicStack *s);

/**
 * @brief Allocates a new node and pushes it onto the top of the stack.
 * @param s    Pointer to the DynamicStack.
 * @param elem Integer value to push.
 * @return 1 if the element was pushed successfully, 0 if malloc failed.
 */
int ds_push(DynamicStack *s, int elem);

/**
 * @brief Removes, frees, and returns the topmost element.
 * @param s   Pointer to the DynamicStack.
 * @param out Pointer to an int that receives the popped value on success.
 *            Unchanged on failure.
 * @return 1 if an element was popped, 0 if the stack is empty.
 */
int ds_pop(DynamicStack *s, int *out);

/**
 * @brief Inspects the topmost element without removing it.
 * @param s   Pointer to the DynamicStack.
 * @param out Pointer to an int that receives the peeked value on success.
 *            Unchanged on failure.
 * @return 1 if the stack is non-empty, 0 otherwise.
 */
int ds_peek(const DynamicStack *s, int *out);

/**
 * @brief Checks whether the stack contains no elements.
 * @param s Pointer to the DynamicStack.
 * @return 1 if empty, 0 otherwise.
 */
int ds_isEmpty(const DynamicStack *s);

/**
 * @brief Returns the number of elements currently in the stack.
 * @param s Pointer to the DynamicStack.
 * @return Element count (>= 0).
 */
int ds_size(const DynamicStack *s);

/**
 * @brief Prints all elements from top to bottom, one per line.
 * @param s Pointer to the DynamicStack.
 */
void ds_print(const DynamicStack *s);

/**
 * @brief Pops and frees every remaining node, resetting the stack to empty.
 *
 * Must be called when the stack is no longer needed to prevent memory leaks.
 *
 * @param s Pointer to the DynamicStack.
 */
void ds_free(DynamicStack *s);
