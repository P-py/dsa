/**
 * @file staticstack.h
 * @brief Interface for the StaticStack Abstract Data Type.
 *
 * A StaticStack is a LIFO (Last-In, First-Out) collection backed by a fixed-size
 * array. Its capacity is determined at compile time by STACKSIZE. Once the stack
 * reaches that limit, push operations are rejected (overflow). Attempting to pop
 * or peek an empty stack is likewise rejected (underflow).
 *
 * All mutating operations report success (1) or failure (0) as their return value
 * so the caller can decide how to handle overflow and underflow without crashing.
 */

#define STACKSIZE 50

/**
 * @brief Represents a fixed-capacity LIFO stack of integers.
 *
 * @note Do not access or modify the fields directly; use the provided functions.
 *
 * Fields:
 *   items - storage array for the elements.
 *   top   - index of the topmost element; -1 when the stack is empty.
 */
typedef struct {
    int items[STACKSIZE];
    int top;
} StaticStack;

/**
 * @brief Initialises a StaticStack, making it ready for use.
 * @param s Pointer to the StaticStack to initialise. Must not be NULL.
 */
void st_init(StaticStack *s);

/**
 * @brief Pushes an element onto the top of the stack.
 * @param s    Pointer to the StaticStack.
 * @param elem Integer value to push.
 * @return 1 if the element was pushed successfully, 0 if the stack is full.
 */
int st_push(StaticStack *s, int elem);

/**
 * @brief Removes and returns the topmost element.
 * @param s   Pointer to the StaticStack.
 * @param out Pointer to an int that receives the popped value on success.
 *            Unchanged on failure.
 * @return 1 if an element was popped, 0 if the stack is empty.
 */
int st_pop(StaticStack *s, int *out);

/**
 * @brief Inspects the topmost element without removing it.
 * @param s   Pointer to the StaticStack.
 * @param out Pointer to an int that receives the peeked value on success.
 *            Unchanged on failure.
 * @return 1 if the stack is non-empty, 0 otherwise.
 */
int st_peek(const StaticStack *s, int *out);

/**
 * @brief Checks whether the stack contains no elements.
 * @param s Pointer to the StaticStack.
 * @return 1 if empty, 0 otherwise.
 */
int st_isEmpty(const StaticStack *s);

/**
 * @brief Checks whether the stack has reached its maximum capacity.
 * @param s Pointer to the StaticStack.
 * @return 1 if full, 0 otherwise.
 */
int st_isFull(const StaticStack *s);

/**
 * @brief Returns the number of elements currently in the stack.
 * @param s Pointer to the StaticStack.
 * @return Element count (0 … STACKSIZE).
 */
int st_size(const StaticStack *s);

/**
 * @brief Prints all elements from top to bottom, one per line.
 * @param s Pointer to the StaticStack.
 */
void st_print(const StaticStack *s);
