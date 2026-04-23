/*
 * Aplicacao interativa para testar a ABB (Arvore Binaria de Busca).
 *
 * Menu em portugues com todas as operacoes definidas em abb.h, alem de uma
 * visualizacao em ASCII da arvore (de lado: maiores no topo, raiz a esquerda).
 *
 * Compilar:
 *   gcc -Wall -Wextra -g abb/abb.c abb/main.c -o build/abb
 *
 * Executar:
 *   ./build/abb
 */

#include <string.h>
#include "abb.h"

#define EXIT_OPTION 13

/* ---------- Utilitarios locais (nao fazem parte do abb.h) ---------- */

/* Libera recursivamente todos os nos da arvore (pos-ordem) */
static void libera_arvore(Arvore *p) {
    if (*p == NULL) return;
    libera_arvore(&(**p).esq);
    libera_arvore(&(**p).dir);
    free(*p);
    *p = NULL;
}

/* Altura da arvore: folha = 0, vazia = -1. */
static int altura(Arvore p) {
    if (p == NULL) return -1;
    int e = altura((*p).esq);
    int d = altura((*p).dir);
    return 1 + (e > d ? e : d);
}

/* Formata uma chave como "[NN]" em 4 caracteres.
 * Valores fora de [-9, 99] nao cabem — mostramos "[??]" no lugar. */
static void formata_no(int v, char dst[4]) {
    if (v >= -9 && v <= 99) {
        char tmp[8];
        snprintf(tmp, sizeof(tmp), "[%2d]", v);
        memcpy(dst, tmp, 4);
    } else {
        memcpy(dst, "[??]", 4);
    }
}

/* Imprime o conteudo de buf removendo espacos a direita. */
static void imprime_linha(const char *buf, int largura) {
    int fim = largura;
    while (fim > 0 && buf[fim - 1] == ' ') fim--;
    printf("%.*s\n", fim, buf);
}

/* Desenho top-down (raiz no topo, folhas embaixo), com nos em "[NN]" e
 * conectores "/" e "\" indicando o caminho para cada filho.
 *
 * Estrategia: BFS por nivel mantendo um vetor de ponteiros, onde NULL
 * indica "slot vazio" (necessario para preservar o alinhamento quando
 * um no nao tem um dos filhos).
 *
 * Em cada nivel d usamos (com W = largura de cada slot em caracteres):
 *   padding inicial = (2^(h-d)   - 1) * W
 *   espacamento     = (2^(h-d+1) - 1) * W
 * Isso garante que cada pai fique centralizado sobre seus dois filhos.
 *
 * Entre dois niveis, o conector fica na metade do caminho pai-filho:
 *   offset = W * 2^(h-d-1) / 2
 * "/" vai a esquerda do centro do pai; "\" a direita. */
static void desenha(Arvore p) {
    int h = altura(p);

    /* arvores muito altas explodem o vetor (2^(h+1) slots) e o largura
     * da linha passa a nao caber no terminal. */
    if (h > 6) {
        printf("(arvore alta demais para o desenho top-down — altura = %d)\n", h);
        return;
    }

    const int W       = 4;                               /* largura do slot */
    const int cap     = 1 << (h + 1);                    /* 2^(h+1)         */
    const int largura = ((1 << (h + 1)) - 1) * W;        /* chars por linha */

    No  **nivel = (No **)calloc(cap, sizeof(No *));
    char *buf   = (char *)malloc(largura + 1);
    if (nivel == NULL || buf == NULL) {
        free(nivel);
        free(buf);
        return;
    }

    nivel[0] = p;
    int qtd = 1;

    for (int d = 0; d <= h; d++) {
        int pad = ((1 << (h - d))     - 1) * W;
        int gap = ((1 << (h - d + 1)) - 1) * W;

        /* --- Linha dos nos: escreve "[NN]" em cada slot ocupado --- */
        memset(buf, ' ', largura);
        for (int i = 0; i < qtd; i++) {
            if (nivel[i] != NULL) {
                int slot = pad + i * (W + gap);
                formata_no((*nivel[i]).info, buf + slot);
            }
        }
        imprime_linha(buf, largura);

        /* --- Linha dos conectores: "/" e "\" abaixo de cada pai --- */
        if (d < h) {
            memset(buf, ' ', largura);
            int off = W * (1 << (h - d - 1)) / 2;

            for (int i = 0; i < qtd; i++) {
                if (nivel[i] == NULL) continue;
                No *n    = nivel[i];
                int slot = pad + i * (W + gap);
                int lp   = slot + (W / 2 - 1) - off;   /* antes do centro */
                int rp   = slot + (W / 2)     + off;   /* depois do centro */

                if ((*n).esq != NULL && lp >= 0 && lp < largura) buf[lp] = '/';
                if ((*n).dir != NULL && rp >= 0 && rp < largura) buf[rp] = '\\';
            }
            imprime_linha(buf, largura);
        }

        /* --- Expande para o proximo nivel --- */
        if (d < h) {
            /* Itera de tras para frente para nao sobrescrever slots antes de le-los */
            for (int i = qtd - 1; i >= 0; i--) {
                No *n = nivel[i];
                nivel[2 * i + 1] = (n != NULL) ? (*n).dir : NULL;
                nivel[2 * i]     = (n != NULL) ? (*n).esq : NULL;
            }
            qtd *= 2;
        }
    }

    free(buf);
    free(nivel);
}

static void visualizar(Arvore p) {
    if (p == NULL) {
        printf("(arvore vazia)\n");
        return;
    }
    printf("Arvore (raiz no topo):\n");
    desenha(p);
}

/* ---------- Menu ---------- */

static void menu(void) {
    printf("=== ABB - Arvore Binaria de Busca ===\n");
    printf(" 1. Inserir (recursivo)\n");
    printf(" 2. Inserir (iterativo)\n");
    printf(" 3. Remover\n");
    printf(" 4. Buscar (recursivo)\n");
    printf(" 5. Buscar (iterativo)\n");
    printf(" 6. Percurso em ordem\n");
    printf(" 7. Percurso pre-ordem\n");
    printf(" 8. Percurso pos-ordem\n");
    printf(" 9. Percurso em largura\n");
    printf("10. Verificar se eh uma ABB\n");
    printf("11. Visualizar arvore\n");
    printf("12. Esvaziar arvore\n");
    printf("13. Sair\n");
}

int main(void) {
    Arvore t;
    cria_arvore(&t);

    printf("\nABB inicializada!\n\n");

    int opt = -1;
    while (opt != EXIT_OPTION) {
        menu();
        printf("Digite uma opcao: ");
        scanf("%d", &opt);
        system("clear");

        int valor;

        switch (opt) {
            case 1: /* insere recursivo */
                printf("Valor a inserir: ");
                scanf("%d", &valor);
                if (insere(&t, valor)) {
                    printf("Valor %d inserido (recursivo).\n", valor);
                } else {
                    printf("Falha: %d ja existe ou sem memoria.\n", valor);
                }
                break;

            case 2: /* insere iterativo */
                printf("Valor a inserir: ");
                scanf("%d", &valor);
                if (n_rec_insere(&t, valor)) {
                    printf("Valor %d inserido (iterativo).\n", valor);
                } else {
                    printf("Falha: %d ja existe ou sem memoria.\n", valor);
                }
                break;

            case 3: /* remover */
                printf("Valor a remover: ");
                scanf("%d", &valor);
                if (remove_arv(&t, valor)) {
                    printf("Valor %d removido.\n", valor);
                } else {
                    printf("Valor %d nao encontrado.\n", valor);
                }
                break;

            case 4: /* busca recursiva */
                printf("Valor a buscar: ");
                scanf("%d", &valor);
                printf("Valor %d %s (recursivo).\n",
                       valor, busca(t, valor) ? "ENCONTRADO" : "NAO encontrado");
                break;

            case 5: /* busca iterativa */
                printf("Valor a buscar: ");
                scanf("%d", &valor);
                printf("Valor %d %s (iterativo).\n",
                       valor, n_rec_busca(t, valor) ? "ENCONTRADO" : "NAO encontrado");
                break;

            case 6: /* in-order */
                if (t == NULL) {
                    printf("(arvore vazia)\n");
                } else {
                    printf("Em ordem: ");
                    inorder(t);
                    printf("\n");
                }
                break;

            case 7: /* pre-order */
                if (t == NULL) {
                    printf("(arvore vazia)\n");
                } else {
                    printf("Pre-ordem: ");
                    preorder(t);
                    printf("\n");
                }
                break;

            case 8: /* post-order */
                if (t == NULL) {
                    printf("(arvore vazia)\n");
                } else {
                    printf("Pos-ordem: ");
                    postorder(t);
                    printf("\n");
                }
                break;

            case 9: /* largura / BFS */
                if (t == NULL) {
                    printf("(arvore vazia)\n");
                } else {
                    printf("Em largura: ");
                    largura(t);
                    printf("\n");
                }
                break;

            case 10: /* verifica ABB */
                printf("A estrutura %s uma ABB valida.\n",
                       verifica_busca(t) ? "EH" : "NAO EH");
                break;

            case 11: /* visualizar */
                visualizar(t);
                break;

            case 12: /* esvaziar */
                libera_arvore(&t);
                printf("Arvore esvaziada.\n");
                break;

            case EXIT_OPTION:
                libera_arvore(&t); /* libera memoria antes de sair */
                printf("Memoria liberada. Encerrando...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

        if (opt != EXIT_OPTION && opt != 11) {
            printf("\n");
            visualizar(t);
        }

        printf("\n");
    }

    return 0;
}
