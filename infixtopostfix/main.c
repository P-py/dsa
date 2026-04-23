/**
 * @file main.c
 * @brief Interactive demo application for infix-to-postfix conversion.
 *
 * The user types an infix expression (integers, +  -  *  /  ^, parentheses).
 * The program converts it to postfix using the Shunting Yard algorithm and
 * then evaluates the result, printing each step so the transformation is clear.
 *
 * Example session:
 *   Digite uma expressao infixa: (3 + 4) * 2
 *   Posfixa : 3 4 + 2 *
 *   Resultado: 14
 *
 * Compile:
 *   gcc -Wall -Wextra -g infixtopostfix/infixtopostfix.c infixtopostfix/main.c -o build/infixtopostfix
 *
 * Run:
 *   ./build/infixtopostfix
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "infixtopostfix.h"

#define EXIT_OPTION 2

/* Forward declarations */
void menu(void);
void run_conversion(void);

int main(void) {
    printf("\n=== Conversor Infixa → Posfixa ===\n");
    printf("Operadores suportados: + - * / ^ (e parenteses)\n");
    printf("Operandos: numeros inteiros (podem ter multiplos digitos)\n\n");

    int opt = -1;
    while (opt != EXIT_OPTION) {
        menu();
        printf("Digite uma opcao: ");
        scanf("%d", &opt);

        /* consume the newline left in the buffer so fgets works correctly */
        getchar();

        system("clear");

        switch (opt) {
            case 1:
                run_conversion();
                break;

            case EXIT_OPTION:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

        printf("\n");
    }

    return 0;
}

/**
 * @brief Reads an infix expression, converts it to postfix, and evaluates it.
 *
 * Handles all error codes from itp_convert() and itp_evaluate() and prints
 * a descriptive message so the user knows what went wrong.
 */
void run_conversion(void) {
    char infix[ITP_MAX_EXPR];
    char postfix[ITP_MAX_EXPR];

    printf("Digite uma expressao infixa: ");
    fgets(infix, sizeof(infix), stdin);

    /* strip the trailing newline that fgets includes */
    int len = (int)strlen(infix);
    if (len > 0 && infix[len - 1] == '\n') {
        infix[len - 1] = '\0';
    }

    printf("\n");
    printf("Infixa  : %s\n", infix);

    /* ── Step 1: convert ─────────────────────────────────────────────── */
    int err = itp_convert(infix, postfix, sizeof(postfix));
    if (err != ITP_OK) {
        printf("Erro na conversao: %s\n", itp_strerror(err));
        return;
    }
    printf("Posfixa : %s\n", postfix);

    /* ── Step 2: evaluate ────────────────────────────────────────────── */
    int result;
    err = itp_evaluate(postfix, &result);
    if (err != ITP_OK) {
        printf("Erro na avaliacao: %s\n", itp_strerror(err));
        return;
    }
    printf("Resultado: %d\n", result);
}

/**
 * @brief Prints the main menu options to stdout.
 */
void menu(void) {
    printf("=== Menu ===\n");
    printf("1. Converter e avaliar expressao\n");
    printf("2. Sair\n");
}
