/**
 * @file bst.c
 * @brief Implementation of the BST (Binary Search Tree) Abstract Data Type.
 *
 * The tree is maintained through heap-allocated nodes reachable from a single
 * root pointer. All navigation uses the BST ordering invariant:
 *
 *   left subtree keys < node key < right subtree keys
 *
 * Example tree after inserting 50, 30, 70, 20, 40, 60, 80:
 *
 *              50
 *             /  \
 *           30    70
 *          /  \  /  \
 *        20  40 60   80
 *
 * Insertion descends from the root comparing keys until a NULL slot is found.
 * Removal handles three cases — leaf, single child, and two children (the node
 * is replaced by its in-order successor, i.e. the leftmost node of its right
 * subtree). All recursive helpers return the (possibly updated) root of the
 * subtree they were called on, which keeps parent-link bookkeeping local.
 */

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/**
 * @brief Allocates a new leaf node with the given key.
 * @return Pointer to the new node, or NULL on allocation failure.
 */
static BSTNode *bst_newNode(int elem) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (node == NULL) {
        return NULL;
    }
    (*node).data  = elem;
    (*node).left  = NULL;
    (*node).right = NULL;
    return node;
}

/**
 * @brief Recursive insert helper.
 *
 * @param root    Current subtree root (may be NULL).
 * @param elem    Key to insert.
 * @param success Out-parameter: set to 1 on insert, 0 on duplicate/malloc fail.
 * @return The (possibly new) root of this subtree.
 */
static BSTNode *bst_insertNode(BSTNode *root, int elem, int *success) {
    if (root == NULL) {
        BSTNode *node = bst_newNode(elem);
        *success = (node != NULL);
        return node;
    }

    if (elem < (*root).data) {
        (*root).left = bst_insertNode((*root).left, elem, success);
    } else if (elem > (*root).data) {
        (*root).right = bst_insertNode((*root).right, elem, success);
    } else {
        /* duplicate — leave the tree untouched */
        *success = 0;
    }
    return root;
}

/**
 * @brief Finds the node holding the smallest key in a non-empty subtree.
 */
static BSTNode *bst_minNode(BSTNode *root) {
    while ((*root).left != NULL) {
        root = (*root).left;
    }
    return root;
}

/**
 * @brief Recursive remove helper.
 *
 * @param root    Current subtree root (may be NULL).
 * @param elem    Key to remove.
 * @param success Out-parameter: set to 1 if a node was removed, 0 otherwise.
 * @return The (possibly new) root of this subtree.
 */
static BSTNode *bst_removeNode(BSTNode *root, const int elem, int *success) {
    if (root == NULL) {
        *success = 0; /* key not found */
        return NULL;
    }

    if (elem < (*root).data) {
        (*root).left = bst_removeNode((*root).left, elem, success);
    } else if (elem > (*root).data) {
        (*root).right = bst_removeNode((*root).right, elem, success);
    } else {
        /* match — splice this node out */
        *success = 1;

        if ((*root).left == NULL) {
            /* 0 or 1 child on the right: replace node by its right child */
            BSTNode *right = (*root).right;
            free(root);
            return right;
        }
        if ((*root).right == NULL) {
            /* exactly 1 child on the left: replace node by its left child */
            BSTNode *left = (*root).left;
            free(root);
            return left;
        }

        /* two children: copy in-order successor's key down, then remove it */
        BSTNode *succ = bst_minNode((*root).right);
        (*root).data  = (*succ).data;
        (*root).right = bst_removeNode((*root).right, (*succ).data, success);
        /* the recursive call above resets *success, but the removal did happen */
        *success = 1;
    }
    return root;
}

static int bst_heightNode(const BSTNode *root) {
    if (root == NULL) {
        return -1;
    }
    int lh = bst_heightNode((*root).left);
    int rh = bst_heightNode((*root).right);
    return 1 + (lh > rh ? lh : rh);
}

static void bst_inorderNode(const BSTNode *root) {
    if (root == NULL) {
        return;
    }
    bst_inorderNode((*root).left);
    printf("%d ", (*root).data);
    bst_inorderNode((*root).right);
}

static void bst_preorderNode(const BSTNode *root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", (*root).data);
    bst_preorderNode((*root).left);
    bst_preorderNode((*root).right);
}

static void bst_postorderNode(const BSTNode *root) {
    if (root == NULL) {
        return;
    }
    bst_postorderNode((*root).left);
    bst_postorderNode((*root).right);
    printf("%d ", (*root).data);
}

/**
 * @brief Sideways ASCII dump — reverse in-order walk so bigger keys sit on top.
 */
static void bst_printNode(const BSTNode *root, int depth) {
    if (root == NULL) {
        return;
    }
    bst_printNode((*root).right, depth + 1);

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    printf("%d\n", (*root).data);

    bst_printNode((*root).left, depth + 1);
}

static void bst_freeNode(BSTNode *root) {
    if (root == NULL) {
        return;
    }
    bst_freeNode((*root).left);
    bst_freeNode((*root).right);
    free(root);
}

/* ---------------------------- public API ---------------------------- */

void bst_init(BST *t) {
    (*t).root = NULL;
    (*t).size = 0;
}

int bst_insert(BST *t, int elem) {
    int success = 0;
    (*t).root = bst_insertNode((*t).root, elem, &success);
    if (success) {
        (*t).size++;
    }
    return success;
}

int bst_search(const BST *t, int elem) {
    BSTNode *cur = (*t).root;
    while (cur != NULL) {
        if (elem == (*cur).data) {
            return 1;
        }
        cur = (elem < (*cur).data) ? (*cur).left : (*cur).right;
    }
    return 0;
}

int bst_remove(BST *t, int elem) {
    int success = 0;
    (*t).root = bst_removeNode((*t).root, elem, &success);
    if (success) {
        (*t).size--;
    }
    return success;
}

int bst_min(const BST *t, int *out) {
    if (bst_isEmpty(t)) {
        return 0;
    }
    *out = (*bst_minNode((*t).root)).data;
    return 1;
}

int bst_max(const BST *t, int *out) {
    if (bst_isEmpty(t)) {
        return 0;
    }
    BSTNode *cur = (*t).root;
    while ((*cur).right != NULL) {
        cur = (*cur).right;
    }
    *out = (*cur).data;
    return 1;
}

int bst_size(const BST *t) {
    return (*t).size;
}

int bst_height(const BST *t) {
    return bst_heightNode((*t).root);
}

int bst_isEmpty(const BST *t) {
    return (*t).root == NULL;
}

void bst_inorder(const BST *t) {
    if (bst_isEmpty(t)) {
        printf("(arvore vazia)\n");
        return;
    }
    printf("In-ordem: ");
    bst_inorderNode((*t).root);
    printf("\n");
}

void bst_preorder(const BST *t) {
    if (bst_isEmpty(t)) {
        printf("(arvore vazia)\n");
        return;
    }
    printf("Pre-ordem: ");
    bst_preorderNode((*t).root);
    printf("\n");
}

void bst_postorder(const BST *t) {
    if (bst_isEmpty(t)) {
        printf("(arvore vazia)\n");
        return;
    }
    printf("Pos-ordem: ");
    bst_postorderNode((*t).root);
    printf("\n");
}

void bst_print(const BST *t) {
    if (bst_isEmpty(t)) {
        printf("(arvore vazia)\n");
        return;
    }
    printf("Arvore (raiz a esquerda, maiores no topo):\n");
    bst_printNode((*t).root, 0);
}

void bst_free(BST *t) {
    bst_freeNode((*t).root);
    (*t).root = NULL;
    (*t).size = 0;
}
