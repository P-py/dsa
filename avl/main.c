/**
 * @file main.c
 * @brief Demonstração interativa da Árvore AVL.
 *
 * Menu que permite exercitar todas as operações do TAD: inserção (recursiva e
 * iterativa), remoção, busca (recursiva e iterativa), percurso in-order,
 * consulta de altura e verificações de validade AVL / ABB.
 *
 * Compile:
 *   gcc -Wall -Wextra -g avl/avl.c avl/main.c -o avl_app
 *
 * Run:
 *   ./avl_app
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

#define OPCAO_SAIR 10

#define CANVAS_COLS 110
#define CANVAS_ROWS 32

static char canvas[CANVAS_ROWS][CANVAS_COLS + 1];

void menu(void);
void exibe_arvore(Arvore p);

int main(void) {
    Arvore arv;
    cria_arvore(&arv);

    printf("\nArvore AVL inicializada!\n\n");

    int opt = -1;
    while (opt != OPCAO_SAIR) {
        menu();
        printf("Digite uma opcao: ");
        scanf("%d", &opt);
        system("clear");

        int chave, resultado;

        switch (opt) {
            case 1: /* inserir — recursivo */
                printf("Valor a inserir: ");
                scanf("%d", &chave);
                resultado = insere(&arv, chave);
                if (resultado)
                    printf("Valor %d inserido com sucesso.\n", chave);
                else
                    printf("Erro: valor %d ja existe na arvore.\n", chave);
                break;

            case 2: /* inserir — nao recursivo */
                printf("Valor a inserir (nao-rec): ");
                scanf("%d", &chave);
                resultado = n_rec_insere(&arv, chave);
                if (resultado)
                    printf("Valor %d inserido com sucesso (nao-rec).\n", chave);
                else
                    printf("Erro: valor %d ja existe na arvore.\n", chave);
                break;

            case 3: /* remover */
                printf("Valor a remover: ");
                scanf("%d", &chave);
                resultado = remove_arv(&arv, chave);
                if (resultado)
                    printf("Valor %d removido com sucesso.\n", chave);
                else
                    printf("Erro: valor %d nao encontrado na arvore.\n", chave);
                break;

            case 4: /* buscar — recursivo */
                printf("Valor a buscar: ");
                scanf("%d", &chave);
                resultado = busca(arv, chave);
                printf("Valor %d %s (busca recursiva).\n",
                       chave, resultado ? "encontrado" : "nao encontrado");
                break;

            case 5: /* buscar — nao recursivo */
                printf("Valor a buscar (nao-rec): ");
                scanf("%d", &chave);
                resultado = n_rec_busca(arv, chave);
                printf("Valor %d %s (busca iterativa).\n",
                       chave, resultado ? "encontrado" : "nao encontrado");
                break;

            case 6: /* percurso in-order */
                printf("In-order: ");
                if (arv == NULL)
                    printf("(arvore vazia)");
                else
                    inorder(arv);
                printf("\n");
                break;

            case 7: /* altura */
                printf("Altura da arvore: %d\n", altura(arv));
                break;

            case 8: /* verificar AVL */
                resultado = verifica_AVL(arv);
                printf("A arvore %s uma AVL valida.\n",
                       resultado ? "E" : "NAO E");
                break;

            case 9: /* verificar ABB */
                resultado = verifica_busca(arv);
                printf("A arvore %s uma arvore de busca (ABB) valida.\n",
                       resultado ? "E" : "NAO E");
                break;

            case OPCAO_SAIR:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

        /* exibe a árvore em formato visual após cada operação */
        if (opt >= 1 && opt <= 9 && opt != 6) {
            printf("\nArvore atual:\n");
            if (arv == NULL)
                printf("  (vazia)\n");
            else
                exibe_arvore(arv);
        }

        printf("\n");
    }

    return 0;
}

void menu(void) {
    printf("=== Arvore AVL ===\n");
    printf(" 1. Inserir (recursivo)\n");
    printf(" 2. Inserir (nao recursivo)\n");
    printf(" 3. Remover\n");
    printf(" 4. Buscar (recursivo)\n");
    printf(" 5. Buscar (nao recursivo)\n");
    printf(" 6. Exibir in-order\n");
    printf(" 7. Ver altura\n");
    printf(" 8. Verificar se e AVL\n");
    printf(" 9. Verificar se e ABB\n");
    printf("10. Sair\n");
}

static void canvas_escreve(int lin, int col, const char *s) {
    for (int i = 0; s[i] && col + i >= 0 && col + i < CANVAS_COLS; i++)
        canvas[lin][col + i] = s[i];
}

static void desenha_no(Arvore p, int lin, int col, int gap) {
    if (p == NULL || lin >= CANVAS_ROWS) return;

    char buf[16];
    snprintf(buf, sizeof(buf), "(%d)[%d]", p->info, p->fb);
    canvas_escreve(lin, col - (int)strlen(buf) / 2, buf);

    if (lin + 2 < CANVAS_ROWS) {
        if (p->esq) {
            canvas[lin + 1][col - gap / 2] = '/';
            desenha_no(p->esq, lin + 2, col - gap, gap / 2 > 1 ? gap / 2 : 1);
        }
        if (p->dir) {
            canvas[lin + 1][col + gap / 2] = '\\';
            desenha_no(p->dir, lin + 2, col + gap, gap / 2 > 1 ? gap / 2 : 1);
        }
    }
}

void exibe_arvore(Arvore raiz) {
    int h      = altura(raiz);
    int linhas = 2 * h + 1;
    if (linhas > CANVAS_ROWS) linhas = CANVAS_ROWS;

    /* gap mínimo: 3 * 2^(h-1) garante que labels de até 5 chars não se sobreponham
       nas folhas (cada par de irmãos fica 2*gap_folha apart, gap_folha >= 3). */
    int gap = h > 0 ? 5 * (1 << (h - 1)) : 3;
    while (gap * 4 > CANVAS_COLS && gap > 2) gap /= 2;

    /* raiz posicionada de modo que a sub-árvore comece perto da coluna 2 */
    int col_raiz = gap * 2 + 2;

    for (int r = 0; r < linhas; r++) {
        memset(canvas[r], ' ', CANVAS_COLS);
        canvas[r][CANVAS_COLS] = '\0';
    }

    desenha_no(raiz, 0, col_raiz, gap);

    for (int r = 0; r < linhas; r++) {
        int len = CANVAS_COLS;
        while (len > 0 && canvas[r][len - 1] == ' ') len--;
        canvas[r][len] = '\0';
        printf("%s\n", canvas[r]);
    }
}
