/**
 * @file main.c
 * @brief Interactive demo application for the BST ADT.
 *
 * Menu-driven interface that exercises every BST operation. bst_free() is
 * called on exit to release all heap memory cleanly.
 *
 * Compile:
 *   gcc -Wall -Wextra -g bst/bst.c bst/main.c -o build/bst
 *
 * Run:
 *   ./build/bst
 */

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

#define EXIT_OPTION 13

/* Forward declarations */
void menu(void);

int main(void) {
    BST tree;
    bst_init(&tree);

    printf("\nArvore Binaria de Busca (BST) inicializada!\n\n");

    int opt = -1;
    while (opt != EXIT_OPTION) {
        menu();
        printf("Digite uma opcao: ");
        scanf("%d", &opt);
        system("clear");

        int elem, result;

        switch (opt) {
            case 1: /* insert */
                printf("Digite o valor a inserir: ");
                scanf("%d", &elem);
                if (bst_insert(&tree, elem)) {
                    printf("Valor %d inserido com sucesso.\n", elem);
                } else {
                    printf("Erro: valor %d ja existe ou falha de memoria.\n", elem);
                }
                break;

            case 2: /* remove */
                printf("Digite o valor a remover: ");
                scanf("%d", &elem);
                if (bst_remove(&tree, elem)) {
                    printf("Valor %d removido com sucesso.\n", elem);
                } else {
                    printf("Erro: valor %d nao encontrado.\n", elem);
                }
                break;

            case 3: /* search */
                printf("Digite o valor a buscar: ");
                scanf("%d", &elem);
                if (bst_search(&tree, elem)) {
                    printf("Valor %d ESTA presente na arvore.\n", elem);
                } else {
                    printf("Valor %d NAO esta presente na arvore.\n", elem);
                }
                break;

            case 4: /* min */
                if (bst_min(&tree, &result)) {
                    printf("Menor valor: %d\n", result);
                } else {
                    printf("Arvore vazia — sem minimo.\n");
                }
                break;

            case 5: /* max */
                if (bst_max(&tree, &result)) {
                    printf("Maior valor: %d\n", result);
                } else {
                    printf("Arvore vazia — sem maximo.\n");
                }
                break;

            case 6: /* size */
                printf("Numero de elementos: %d\n", bst_size(&tree));
                break;

            case 7: /* height */
                printf("Altura da arvore: %d\n", bst_height(&tree));
                break;

            case 8: /* in-order */
                bst_inorder(&tree);
                break;

            case 9: /* pre-order */
                bst_preorder(&tree);
                break;

            case 10: /* post-order */
                bst_postorder(&tree);
                break;

            case 11: /* print tree */
                bst_print(&tree);
                break;

            case 12: /* clear */
                bst_free(&tree);
                printf("Arvore esvaziada.\n");
                break;

            case EXIT_OPTION:
                bst_free(&tree); /* release all heap nodes before exiting */
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
    printf("=== Arvore Binaria de Busca ===\n");
    printf("1.  Inserir (insert)\n");
    printf("2.  Remover (remove)\n");
    printf("3.  Buscar (search)\n");
    printf("4.  Menor valor (min)\n");
    printf("5.  Maior valor (max)\n");
    printf("6.  Tamanho\n");
    printf("7.  Altura\n");
    printf("8.  Percurso in-ordem\n");
    printf("9.  Percurso pre-ordem\n");
    printf("10. Percurso pos-ordem\n");
    printf("11. Exibir arvore (ASCII)\n");
    printf("12. Esvaziar arvore\n");
    printf("13. Sair\n");
}
