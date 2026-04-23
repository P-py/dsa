#include <stdio.h>
#include <stdlib.h>

typedef struct no {
  int info;
  int fb;   /* hesq - hdir */
  struct no *esq, *dir;
} No;

typedef No* Arvore;

void cria_arvore(Arvore *p);

int altura(Arvore p);

void inorder(Arvore p);

int verifica_AVL(Arvore p);

void LL(Arvore *p);
void RR(Arvore *p);
void LR(Arvore *p);
void RL(Arvore *p);

/* Retorna 1 se a chave for encontrada */
int busca (Arvore p, int chave);

/* Retorna 1 se a chave for encontrada */
int n_rec_busca(Arvore p, int chave);

/* Retorna 0 se a chave for repetida */
int  insere(Arvore *p, int chave);

/* Retorna 0 se a chave for repetida */
/* Versão não recursiva */
int  n_rec_insere(Arvore *p, int chave);

/* Retorna 0 se a chave não for encontrada */
int remove_arv(Arvore *p, int chave);

/* Verifica se p é uma árvore de busca */
int verifica_busca(Arvore p);
