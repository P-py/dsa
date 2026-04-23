/**
 * @file bst.h
 * @brief Interface for the BST (Binary Search Tree) Abstract Data Type.
 *
 * A BST stores integer keys such that for every node N:
 *   - every key in N's left subtree is strictly less than N's key
 *   - every key in N's right subtree is strictly greater than N's key
 *
 * This implementation rejects duplicate keys (insert returns 0). Nodes are
 * heap-allocated, so bst_free() MUST be called when the tree is no longer
 * needed to avoid memory leaks.
 *
 * The tree is not self-balancing: the worst-case shape is a degenerate chain
 * (O(n) search/insert/remove) when keys are inserted in sorted order.
 *
 * All mutating operations report success (1) or failure (0) as their return
 * value. Query operations that produce a value through an out-parameter also
 * return 1 on success and 0 when the tree is empty.
 */

#include <stdlib.h>

/**
 * @brief A single node in the Binary Search Tree.
 *
 * Fields:
 *   data  - the integer key stored in this node.
 *   left  - pointer to the root of the left subtree (keys < data), or NULL.
 *   right - pointer to the root of the right subtree (keys > data), or NULL.
 */
typedef struct BSTNode {
    int data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

/**
 * @brief Represents an unbalanced Binary Search Tree of integers.
 *
 * @note Do not access or modify the fields directly; use the provided functions.
 *
 * Fields:
 *   root - pointer to the root node; NULL when the tree is empty.
 *   size - number of nodes currently in the tree (kept in sync by insert/remove).
 */
typedef struct {
    BSTNode *root;
    int size;
} BST;

/**
 * @brief Initialises a BST, making it ready for use.
 * @param t Pointer to the BST to initialise. Must not be NULL.
 */
void bst_init(BST *t);

/**
 * @brief Inserts a new key into the tree.
 *
 * Duplicate keys are rejected (the tree is left unchanged).
 *
 * @param t    Pointer to the BST.
 * @param elem Integer key to insert.
 * @return 1 if inserted, 0 if the key already exists or malloc failed.
 */
int bst_insert(BST *t, int elem);

/**
 * @brief Checks whether a key is present in the tree.
 * @param t    Pointer to the BST.
 * @param elem Integer key to look for.
 * @return 1 if found, 0 otherwise.
 */
int bst_search(const BST *t, int elem);

/**
 * @brief Removes a key from the tree.
 *
 * Handles the three classical cases: leaf, one child, two children
 * (replaced by its in-order successor — the smallest key in the right subtree).
 *
 * @param t    Pointer to the BST.
 * @param elem Integer key to remove.
 * @return 1 if removed, 0 if the key was not found.
 */
int bst_remove(BST *t, int elem);

/**
 * @brief Retrieves the smallest key in the tree.
 * @param t   Pointer to the BST.
 * @param out Pointer that receives the minimum key on success.
 * @return 1 on success, 0 if the tree is empty.
 */
int bst_min(const BST *t, int *out);

/**
 * @brief Retrieves the largest key in the tree.
 * @param t   Pointer to the BST.
 * @param out Pointer that receives the maximum key on success.
 * @return 1 on success, 0 if the tree is empty.
 */
int bst_max(const BST *t, int *out);

/**
 * @brief Returns the number of keys currently stored.
 * @param t Pointer to the BST.
 * @return Element count (>= 0).
 */
int bst_size(const BST *t);

/**
 * @brief Returns the height of the tree.
 *
 * An empty tree has height -1; a tree with only a root has height 0.
 *
 * @param t Pointer to the BST.
 * @return Height of the tree.
 */
int bst_height(const BST *t);

/**
 * @brief Checks whether the tree contains no keys.
 * @param t Pointer to the BST.
 * @return 1 if empty, 0 otherwise.
 */
int bst_isEmpty(const BST *t);

/**
 * @brief Prints the keys in in-order (ascending) order on a single line.
 * @param t Pointer to the BST.
 */
void bst_inorder(const BST *t);

/**
 * @brief Prints the keys in pre-order (root, left, right) on a single line.
 * @param t Pointer to the BST.
 */
void bst_preorder(const BST *t);

/**
 * @brief Prints the keys in post-order (left, right, root) on a single line.
 * @param t Pointer to the BST.
 */
void bst_postorder(const BST *t);

/**
 * @brief Prints an ASCII sideways visualisation of the tree.
 *
 * The right subtree is printed above the root, the left subtree below, with
 * indentation proportional to depth. Reading top-to-bottom at a fixed column
 * reproduces the in-order sequence.
 *
 * @param t Pointer to the BST.
 */
void bst_print(const BST *t);

/**
 * @brief Frees every node in the tree and resets it to empty.
 *
 * Must be called when the tree is no longer needed to prevent memory leaks.
 *
 * @param t Pointer to the BST.
 */
void bst_free(BST *t);
