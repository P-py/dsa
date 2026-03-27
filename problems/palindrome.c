/**
 * @file palindrome.c
 * @brief Mini PBL — Palindrome Checker using a Stack (char variant).
 *
 * A stack reverses the order of whatever is pushed onto it (LIFO).
 * Algorithm:
 *   1. Push every character of the word onto the stack.
 *   2. Walk the word again from the first character; at each position pop
 *      one character and compare with the current character.
 *   3. If every pair matches the word is a palindrome; a single mismatch
 *      is enough to reject it.
 *
 * No strrev or any ready-made reversal function is used — the stack itself
 * provides the reversal through its push/pop operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char        letter;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
    int   size;
} CharStack;

static void cs_init(CharStack *s) {
    (*s).top = NULL;
    (*s).size = 0;
}

/* Returns 1 on success, 0 if malloc fails. */
static int cs_push(CharStack *s, char c) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return 0;
    }
    (*node).letter = c;
    (*node).next = (*s).top;
    (*s).top = node;
    (*s).size++;
    return 1;
}

/* Removes the top node and writes its letter to *out.
 * Returns 1 on success, 0 if the stack is empty. */
static int cs_pop(CharStack *s, char *out) {
    if ((*s).top == NULL) {
        return 0;
    }
    Node *removed = (*s).top;
    *out = (*removed).letter;
    (*s).top = (*removed).next;
    free(removed);
    (*s).size--;
    return 1;
}

static void cs_free(CharStack *s) {
    char discarded;
    while (cs_pop(s, &discarded)) { /* cs_pop frees each node */ }
}

/**
 * @brief Checks whether the null-terminated string @p word is a palindrome.
 *
 * Uses only stack push/pop operations — no string reversal function.
 *
 * @param word  Null-terminated string to test.
 * @return      1 if palindrome, 0 otherwise.
 */
int isPalindrome(const char *word) {
    int len = 0;
    while (word[len] != '\0') { /* manual strlen — no library call */
        len++;
    }

    CharStack s;
    cs_init(&s);

    /* Step 1: push every character */
    for (int i = 0; i < len; i++) {
        if (!cs_push(&s, word[i])) {
            cs_free(&s);
            return 0; /* allocation failure — treat as non-palindrome */
        }
    }

    /* Step 2: pop and compare with original left-to-right
     * The stack yields characters in reverse order (right-to-left),
     * so a mismatch means the word is not symmetric. */
    int palindrome = 1;
    for (int i = 0; i < len; i++) {
        char popped;
        cs_pop(&s, &popped);
        if (popped != word[i]) {
            palindrome = 0;
            break;
        }
    }

    cs_free(&s); /* free any remaining nodes on early exit */
    return palindrome;
}

int main(void) {
    char word[100];

    printf("Digite uma palavra (ou 'sair' para encerrar): ");

    while (scanf("%99s", word) == 1) {
        if (strcmp(word, "sair") == 0) {
            break;
        }

        if (isPalindrome(word)) {
            printf("Palindromo.\n");
        } else {
            printf("Nao Palindromo.\n");
        }

        printf("\nDigite uma palavra (ou 'sair' para encerrar): ");
    }

    printf("Encerrando.\n");
    return 0;
}
