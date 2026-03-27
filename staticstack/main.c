/**
 * @file main.c
 * @brief Interactive demo application for the StaticStack ADT.
 *
 * Provides a menu-driven interface so the user can exercise every StaticStack
 * operation manually and observe the results.
 *
 * Compile:
 *   gcc -Wall -Wextra -g staticstack/staticstack.c staticstack/main.c -o staticstack_app
 *
 * Run:
 *   ./staticstack_app
 */

#include <stdio.h>
#include <stdlib.h>
#include "staticstack.h"

#define EXIT_OPTION 8

/* Forward declarations */
void menu(void);

int main(void) {
    StaticStack stack;
    st_init(&stack);

    printf("\nPilha estatica inicializada! (capacidade: %d)\n\n", STACKSIZE);

    int opt = -1;
    while (opt != EXIT_OPTION) {
        menu();
        printf("Digite uma opcao: ");
        scanf("%d", &opt);
        system("clear");

        int elem, result;

        switch (opt) {
            case 1: /* push */
                printf("Digite o valor a empilhar: ");
                scanf("%d", &elem);
                if (st_push(&stack, elem)) {
                    printf("Valor %d empilhado com sucesso.\n", elem);
                } else {
                    printf("Erro: pilha cheia (overflow).\n");
                }
                break;

            case 2: /* pop */
                if (st_pop(&stack, &result)) {
                    printf("Valor desempilhado: %d\n", result);
                } else {
                    printf("Erro: pilha vazia (underflow).\n");
                }
                break;

            case 3: /* peek */
                if (st_peek(&stack, &result)) {
                    printf("Topo da pilha: %d\n", result);
                } else {
                    printf("Pilha vazia — nao ha topo.\n");
                }
                break;

            case 4: /* size */
                printf("Numero de elementos: %d\n", st_size(&stack));
                break;

            case 5: /* isEmpty */
                printf("A pilha esta %s.\n", st_isEmpty(&stack) ? "vazia" : "nao vazia");
                break;

            case 6: /* isFull */
                printf("A pilha esta %s.\n", st_isFull(&stack) ? "cheia" : "nao cheia");
                break;

            case 7: /* print */
                st_print(&stack);
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
 * @brief Prints the main menu options to stdout.
 */
void menu(void) {
    printf("=== Pilha Estatica ===\n");
    printf("1. Empilhar (push)\n");
    printf("2. Desempilhar (pop)\n");
    printf("3. Ver topo (peek)\n");
    printf("4. Ver tamanho\n");
    printf("5. Verificar se esta vazia\n");
    printf("6. Verificar se esta cheia\n");
    printf("7. Exibir pilha\n");
    printf("8. Sair\n");
}
