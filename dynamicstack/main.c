/**
 * @file main.c
 * @brief Interactive demo application for the DynamicStack ADT.
 *
 * Provides a menu-driven interface so the user can exercise every DynamicStack
 * operation manually and observe the results. ds_free() is called on exit to
 * release all heap memory cleanly.
 *
 * Compile:
 *   gcc -Wall -Wextra -g dynamicstack/dynamicstack.c dynamicstack/main.c -o build/dynamicstack
 *
 * Run:
 *   ./build/dynamicstack
 */

#include <stdio.h>
#include <stdlib.h>
#include "dynamicstack.h"

#define EXIT_OPTION 7

/* Forward declarations */
void menu(void);

int main(void) {
    DynamicStack stack;
    ds_init(&stack);

    printf("\nPilha dinamica inicializada! (sem limite de capacidade)\n\n");

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
                if (ds_push(&stack, elem)) {
                    printf("Valor %d empilhado com sucesso.\n", elem);
                } else {
                    printf("Erro: falha ao alocar memoria (push).\n");
                }
                break;

            case 2: /* pop */
                if (ds_pop(&stack, &result)) {
                    printf("Valor desempilhado: %d\n", result);
                } else {
                    printf("Erro: pilha vazia (underflow).\n");
                }
                break;

            case 3: /* peek */
                if (ds_peek(&stack, &result)) {
                    printf("Topo da pilha: %d\n", result);
                } else {
                    printf("Pilha vazia — nao ha topo.\n");
                }
                break;

            case 4: /* size */
                printf("Numero de elementos: %d\n", ds_size(&stack));
                break;

            case 5: /* isEmpty */
                printf("A pilha esta %s.\n", ds_isEmpty(&stack) ? "vazia" : "nao vazia");
                break;

            case 6: /* print */
                ds_print(&stack);
                break;

            case EXIT_OPTION:
                ds_free(&stack); /* release all heap nodes before exiting */
                printf("Memoria liberada. Encerrando...\n");
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
    printf("=== Pilha Dinamica ===\n");
    printf("1. Empilhar (push)\n");
    printf("2. Desempilhar (pop)\n");
    printf("3. Ver topo (peek)\n");
    printf("4. Ver tamanho\n");
    printf("5. Verificar se esta vazia\n");
    printf("6. Exibir pilha\n");
    printf("7. Sair\n");
}
