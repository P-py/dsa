/*
 * Implementacao das operacoes da Arvore Binaria de Busca (ABB).
 *
 * Convencoes:
 *   - (*p).campo e usado no lugar de p->campo
 *   - funcoes de insercao/remocao retornam 1 em sucesso e 0 em falha
 */

#include "abb.h"

/* Inicializa a arvore como vazia.
 * Lembre que Arvore e um No*, entao comecar com NULL indica "sem raiz". */
void cria_arvore(Arvore *p) {
    *p = NULL;
}

/* Percurso em ordem: esquerda -> raiz -> direita.
 * Em uma ABB valida, imprime as chaves em ordem crescente. */
void inorder(Arvore p) {
    if (p == NULL) return;
    inorder((*p).esq);
    printf("%d ", (*p).info);
    inorder((*p).dir);
}

/* Percurso pre-ordem: raiz -> esquerda -> direita.
 * Util para copiar/serializar a arvore preservando sua estrutura
 * (reinserir as chaves nessa ordem reconstroi a mesma arvore). */
void preorder(Arvore p) {
    if (p == NULL) return;
    printf("%d ", (*p).info);
    preorder((*p).esq);
    preorder((*p).dir);
}

/* Percurso pos-ordem: esquerda -> direita -> raiz.
 * Processa os filhos antes do pai — ideal para liberar memoria
 * ou calcular valores que dependem das subarvores (altura, soma, etc.). */
void postorder(Arvore p) {
    if (p == NULL) return;
    postorder((*p).esq);
    postorder((*p).dir);
    printf("%d ", (*p).info);
}

/* No interno da fila usada pelo percurso em largura */
typedef struct no_fila {
    No              *ref;
    struct no_fila  *prox;
} NoFila;

/* Percurso em largura (BFS): visita os nos nivel a nivel (da raiz para as folhas).
 *
 * Diferente dos percursos in/pre/pos (que sao DFS e usam a pilha de chamadas),
 * o BFS precisa de uma fila explicita para guardar os proximos nos a visitar.
 * A cada iteracao: desenfileira um no, imprime sua chave e enfileira os filhos.
 * Enfileirar esq antes de dir garante impressao da esquerda para a direita. */
void largura(Arvore p) {
    if (p == NULL) return;

    /* cria a fila com o no raiz */
    NoFila *inicio = (NoFila *)malloc(sizeof(NoFila));
    if (inicio == NULL) return;
    (*inicio).ref  = p;
    (*inicio).prox = NULL;
    NoFila *fim = inicio;

    while (inicio != NULL) {
        No *atual = (*inicio).ref;
        printf("%d ", (*atual).info);

        /* enfileira filho esquerdo, depois direito */
        if ((*atual).esq != NULL) {
            NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
            (*novo).ref  = (*atual).esq;
            (*novo).prox = NULL;
            (*fim).prox  = novo;
            fim          = novo;
        }
        if ((*atual).dir != NULL) {
            NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
            (*novo).ref  = (*atual).dir;
            (*novo).prox = NULL;
            (*fim).prox  = novo;
            fim          = novo;
        }

        /* desenfileira o no ja visitado */
        NoFila *tmp = inicio;
        inicio = (*inicio).prox;
        free(tmp);
    }
}

/* Busca recursiva usando a propriedade da ABB.
 * Em cada no descartamos metade da arvore:
 *   chave < info -> so pode estar na esquerda
 *   chave > info -> so pode estar na direita
 * Custo: O(h), onde h e a altura (O(log n) se a arvore estiver balanceada,
 * O(n) no pior caso de uma arvore degenerada em "corrente"). */
int busca(Arvore p, int chave) {
    if (p == NULL) return 0;
    if (chave == (*p).info) return 1;
    if (chave <  (*p).info) return busca((*p).esq, chave);
    return busca((*p).dir, chave);
}

/* Mesma logica da busca recursiva, mas em loop — sem usar a pilha de chamadas.
 * Em arvores muito altas isso evita risco de stack overflow e costuma ser
 * um pouco mais rapido por nao pagar o custo de chamadas de funcao. */
int n_rec_busca(Arvore p, int chave) {
    while (p != NULL) {
        if (chave == (*p).info) return 1;
        p = (chave < (*p).info) ? (*p).esq : (*p).dir;
    }
    return 0;
}

/* Insercao recursiva. Toda nova chave acaba sempre como uma folha.
 *
 * Recebemos um "ponteiro para ponteiro" (Arvore *) porque podemos precisar
 * ALTERAR o proprio ponteiro do pai (caso *p == NULL, e aqui que o novo no
 * entra na arvore). Passar so No* nao permitiria isso.
 *
 * Retornos: 1 = inserido, 0 = chave ja existia ou malloc falhou. */
int insere(Arvore *p, int chave) {
    if (*p == NULL) {
        /* espaco vazio achado: cria o novo no aqui */
        No *novo = (No *)malloc(sizeof(No));
        if (novo == NULL) return 0;
        (*novo).info = chave;
        (*novo).esq  = NULL;
        (*novo).dir  = NULL;
        *p = novo;
        return 1;
    }

    No *raiz = *p;
    if (chave < (*raiz).info) return insere(&(*raiz).esq, chave);
    if (chave > (*raiz).info) return insere(&(*raiz).dir, chave);
    return 0; /* chave repetida — ABB nao aceita duplicatas nesta implementacao */
}

/* Insercao iterativa. Mesma ideia da recursiva, mas guardamos manualmente
 * o "pai" do no onde vamos parar — pois quando achamos o NULL ja perdemos
 * a referencia para o no anterior. */
int n_rec_insere(Arvore *p, int chave) {
    No *pai   = NULL;
    No *atual = *p;

    /* desce ate achar um filho vazio ou encontrar a chave (duplicada) */
    while (atual != NULL) {
        if (chave == (*atual).info) return 0;
        pai   = atual;
        atual = (chave < (*atual).info) ? (*atual).esq : (*atual).dir;
    }

    /* cria o no novo ja como folha */
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) return 0;
    (*novo).info = chave;
    (*novo).esq  = NULL;
    (*novo).dir  = NULL;

    /* conecta o no novo no lugar certo */
    if (pai == NULL) {
        *p = novo;                  /* arvore estava vazia: novo vira raiz */
    } else if (chave < (*pai).info) {
        (*pai).esq = novo;          /* vai como filho esquerdo */
    } else {
        (*pai).dir = novo;          /* vai como filho direito */
    }
    return 1;
}

/* Retorna o no de menor chave em uma subarvore.
 * Em uma ABB, o minimo esta sempre no caminho mais a esquerda possivel.
 * Usada pela remocao para achar o sucessor em ordem (menor da subarvore direita). */
No *encontrar_minimo(No *raiz) {
    if (raiz == NULL) return NULL;
    while ((*raiz).esq != NULL) {
        raiz = (*raiz).esq;
    }
    return raiz;
}

/* Remocao em uma ABB. Primeiro descemos ate achar a chave (igual a busca);
 * ao achar, tratamos TRES casos classicos sem quebrar a propriedade da ABB:
 *
 *   1. No sem filho a esquerda  -> promove o filho da direita (pode ser NULL)
 *   2. No sem filho a direita   -> promove o filho da esquerda
 *   3. No com dois filhos       -> copia a chave do SUCESSOR em ordem
 *      (menor valor da subarvore direita) para este no, e remove o sucessor
 *      recursivamente. O sucessor tem no maximo um filho, entao cai no caso 1.
 *
 * Usar o sucessor preserva a ordem: tudo na esquerda continua menor,
 * tudo na direita continua maior. */
int remove_arv(Arvore *p, int chave) {
    if (*p == NULL) return 0; /* chave nao encontrada */

    No *raiz = *p;

    /* continua descendo enquanto a chave nao for a deste no */
    if (chave < (*raiz).info) return remove_arv(&(*raiz).esq, chave);
    if (chave > (*raiz).info) return remove_arv(&(*raiz).dir, chave);

    /* aqui raiz contem a chave — aplicamos um dos tres casos */
    if ((*raiz).esq == NULL) {
        *p = (*raiz).dir;   /* caso 1: promove filho direito (ou NULL) */
        free(raiz);
    } else if ((*raiz).dir == NULL) {
        *p = (*raiz).esq;   /* caso 2: promove filho esquerdo */
        free(raiz);
    } else {
        /* caso 3: dois filhos. Copia a chave do sucessor para ca e deleta
         * o sucessor original (que tem no maximo 1 filho, caso simples). */
        No *suc = encontrar_minimo((*raiz).dir);
        (*raiz).info = (*suc).info;
        remove_arv(&(*raiz).dir, (*suc).info);
    }
    return 1;
}

/* Helper da verifica_busca.
 *
 * Truque central: uma arvore binaria e uma ABB valida se, e somente se,
 * seu percurso em ordem produzir chaves ESTRITAMENTE CRESCENTES. Entao
 * caminhamos em ordem guardando o ultimo no visitado (anterior) e, a cada
 * novo no, conferimos se a chave atual e maior que a anterior.
 *
 * Essa abordagem e mais simples do que passar limites (min/max) descendo
 * a recursao, e evita sobrecarregar a assinatura da funcao. */
static int verifica_aux(Arvore p, No **anterior) {
    if (p == NULL) return 1;

    /* verifica a subarvore esquerda primeiro (percurso em ordem) */
    if (!verifica_aux((*p).esq, anterior)) return 0;

    /* se ja visitamos algum no antes, o atual tem que ser maior */
    if (*anterior != NULL && (**anterior).info >= (*p).info) return 0;
    *anterior = p;

    /* segue para a subarvore direita */
    return verifica_aux((*p).dir, anterior);
}

/* Retorna 1 se a arvore respeita a propriedade da ABB, 0 caso contrario. */
int verifica_busca(Arvore p) {
    No *anterior = NULL;
    return verifica_aux(p, &anterior);
}
