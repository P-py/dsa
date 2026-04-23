/**
 * @file avl.c
 * @brief Implementação da Árvore AVL (Árvore Binária de Busca Auto-Balanceada).
 *
 * Uma árvore AVL é uma ABB onde, para todo nó p, o fator de balanceamento
 *   fb(p) = altura(p->esq) - altura(p->dir)
 * pertence a {-1, 0, 1}. Quando uma inserção ou remoção quebra esse
 * invariante (fb = ±2), a árvore é reequilibrada via uma das quatro rotações:
 *   LL (simples à direita), RR (simples à esquerda),
 *   LR (dupla esq-dir),     RL (dupla dir-esq).
 *
 * Compile:
 *   gcc -Wall -Wextra -g avl/avl.c avl/main.c -o avl_app
 */

#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

static int maximo(int a, int b) {
    return a > b ? a : b;
}

/* Recalcula o fb de um nó com base nas alturas reais dos filhos. */
static void atualiza_fb(No *no) {
    no->fb = altura(no->esq) - altura(no->dir);
}

/* Declaração antecipada — rebalanceia é chamado por insere/remove. */
static void rebalanceia(Arvore *p);

void cria_arvore(Arvore *p) {
    *p = NULL;
}

int altura(Arvore p) {
    if (p == NULL) return -1;
    return 1 + maximo(altura(p->esq), altura(p->dir));
}

void inorder(Arvore p) {
    if (p == NULL) return;
    inorder(p->esq);
    printf("%d ", p->info);
    inorder(p->dir);
}

int verifica_AVL(Arvore p) {
    if (p == NULL) return 1;
    int fb_calc = altura(p->esq) - altura(p->dir);
    if (fb_calc < -1 || fb_calc > 1) return 0;
    return verifica_AVL(p->esq) && verifica_AVL(p->dir);
}

/*
 * LL — rotação simples à direita (desequilíbrio ESQ-ESQ).
 *
 *      raiz                 q
 *      /  \                / \
 *     q    C    =>        A  raiz
 *    / \                     /  \
 *   A   B                   B    C
 */
void LL(Arvore *p) {
    No *raiz = *p;
    No *q    = raiz->esq;
    raiz->esq = q->dir;
    q->dir    = raiz;
    atualiza_fb(raiz);
    atualiza_fb(q);
    *p = q;
}

/*
 * RR — rotação simples à esquerda (desequilíbrio DIR-DIR).
 *
 *   raiz                   q
 *   /  \                  / \
 *  A    q      =>       raiz  C
 *      / \              /  \
 *     B   C            A    B
 */
void RR(Arvore *p) {
    No *raiz = *p;
    No *q    = raiz->dir;
    raiz->dir = q->esq;
    q->esq    = raiz;
    atualiza_fb(raiz);
    atualiza_fb(q);
    *p = q;
}

/*
 * LR — rotação dupla esquerda-direita (desequilíbrio ESQ-DIR).
 * Passo 1: RR no filho esquerdo.
 * Passo 2: LL na raiz.
 */
void LR(Arvore *p) {
    RR(&(*p)->esq);
    LL(p);
}

/*
 * RL — rotação dupla direita-esquerda (desequilíbrio DIR-ESQ).
 * Passo 1: LL no filho direito.
 * Passo 2: RR na raiz.
 */
void RL(Arvore *p) {
    LL(&(*p)->dir);
    RR(p);
}

static void rebalanceia(Arvore *p) {
    No *raiz = *p;
    if (raiz->fb == 2) {
        if (raiz->esq->fb >= 0)
            LL(p); /* ESQ-ESQ */
        else
            LR(p); /* ESQ-DIR */
    } else if (raiz->fb == -2) {
        if (raiz->dir->fb <= 0)
            RR(p); /* DIR-DIR */
        else
            RL(p); /* DIR-ESQ */
    }
}

int busca(Arvore p, int chave) {
    if (p == NULL) return 0;
    if (chave == p->info) return 1;
    if (chave  < p->info) return busca(p->esq, chave);
    return busca(p->dir, chave);
}

int n_rec_busca(Arvore p, int chave) {
    while (p != NULL) {
        if (chave == p->info) return 1;
        p = (chave < p->info) ? p->esq : p->dir;
    }
    return 0;
}

int insere(Arvore *p, int chave) {
    if (*p == NULL) {
        No *novo = (No *)malloc(sizeof(No));
        if (novo == NULL) return 0;
        novo->info = chave;
        novo->fb   = 0;
        novo->esq  = NULL;
        novo->dir  = NULL;
        *p = novo;
        return 1;
    }

    No *raiz = *p;
    if (chave == raiz->info) return 0; /* chave duplicada */

    int ok;
    if (chave < raiz->info)
        ok = insere(&raiz->esq, chave);
    else
        ok = insere(&raiz->dir, chave);

    if (ok) {
        raiz->fb = altura(raiz->esq) - altura(raiz->dir);
        rebalanceia(p);
    }

    return ok;
}

/*
 * n_rec_insere — inserção iterativa com rebalanceamento.
 *
 * Desce pela árvore guardando os ponteiros de cada ancestral numa pilha.
 * Após inserir o novo nó na folha, atualiza fb e rebalanceia de baixo para cima.
 */
#define AVL_ALTURA_MAX 64

int n_rec_insere(Arvore *raiz, int chave) {
    Arvore *caminho[AVL_ALTURA_MAX];
    int     topo = 0;

    Arvore *atual = raiz;
    while (*atual != NULL) {
        No *no = *atual;
        if (chave == no->info) return 0; /* chave duplicada */
        caminho[topo++] = atual;
        atual = (chave < no->info) ? &no->esq : &no->dir;
    }

    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) return 0;
    novo->info = chave;
    novo->fb   = 0;
    novo->esq  = NULL;
    novo->dir  = NULL;
    *atual = novo;

    /* percorre os ancestrais de baixo para cima atualizando fb */
    for (int i = topo - 1; i >= 0; i--) {
        No *no = *caminho[i];
        no->fb = altura(no->esq) - altura(no->dir);
        rebalanceia(caminho[i]);
    }

    return 1;
}

int remove_arv(Arvore *p, int chave) {
    if (*p == NULL) return 0;

    No *raiz = *p;
    int ok;

    if (chave < raiz->info) {
        ok = remove_arv(&raiz->esq, chave);
    } else if (chave > raiz->info) {
        ok = remove_arv(&raiz->dir, chave);
    } else {
        /* nó encontrado — três casos */
        if (raiz->esq == NULL) {
            *p = raiz->dir;
            free(raiz);
            return 1;
        }
        if (raiz->dir == NULL) {
            *p = raiz->esq;
            free(raiz);
            return 1;
        }
        /* dois filhos: substitui pelo menor nó da sub-árvore direita */
        No *suc = raiz->dir;
        while (suc->esq != NULL)
            suc = suc->esq;
        raiz->info = suc->info;
        ok = remove_arv(&raiz->dir, suc->info);
    }

    if (ok && *p != NULL) {
        No *no = *p;
        no->fb = altura(no->esq) - altura(no->dir);
        rebalanceia(p);
    }

    return ok;
}

/*
 * vb_aux — verifica a propriedade ABB com limites passados recursivamente.
 * Todo nó deve satisfazer: min < info < max (quando os limites existem).
 */
static int vb_aux(Arvore p, int min, int max, int chk_min, int chk_max) {
    if (p == NULL) return 1;
    if (chk_min && p->info <= min) return 0;
    if (chk_max && p->info >= max) return 0;
    return vb_aux(p->esq, min, p->info, chk_min, 1) &&
           vb_aux(p->dir, p->info, max, 1, chk_max);
}

int verifica_busca(Arvore p) {
    return vb_aux(p, 0, 0, 0, 0);
}
