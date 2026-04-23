/*
 * Arvores binarias de busca.
 */


#include <stdio.h>
#include <stdlib.h>

typedef struct no {
  int info; /* chave */
  struct no *esq, *dir;
} No;

typedef No* Arvore;

void cria_arvore(Arvore *p);

/* Percursos */
// Realiza o percurso em ordem na arvore binaria
void inorder(Arvore p);
// Realiza o percurso pre ordem na arvore binaria
void preorder(Arvore p);
// Realiza o percurso pos ordem na arvore binaria
void postorder(Arvore p);
// Realiza o percurso em largura na arvore binaria
void largura(Arvore p);

/* Implementacao recursiva da busca em ABB.
Retorna 1 se a chave for encontrada */
int busca (Arvore p, int chave);

/* Implementacao nao recursiva da busca em ABB.
Retorna 1 se a chave for encontrada */
int n_rec_busca(Arvore p, int chave);

/* Implementacao recursiva da insercao de uma chave na ABB. 
Retorna 0 se a chave for repetida ou 1 se a chave for inserida com sucesso. */
int  insere(Arvore *p, int chave);

/* Implementacao nao recursiva da insercao de uma chave na ABB. 
Retorna 0 se a chave for repetida ou 1 se a chave for inserida com sucesso. */
int  n_rec_insere(Arvore *p, int chave);

// Função auxiliar para encontrar o menor valor na subarvore direita
No *encontrar_minimo(No *raiz);

/* Funcao que remove uma chave da ABB.
Retorna 0 se a chave nao for encontrada */
int remove_arv(Arvore *p, int chave);

/* Verifica se p eh uma arvore de busca */
int verifica_busca(Arvore p);

