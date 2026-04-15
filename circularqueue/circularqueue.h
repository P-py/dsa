/*
Pedro Santos :D
*/

#define QUEUESIZE 100

typedef struct {
    int data[QUEUESIZE];
    int head;
    int tail;
    int count;
} CircularQueue;

/* Inicializa a fila*/
void cq_init(CircularQueue *q);
/* Destrói a fila */
void cq_destroy(CircularQueue *q);
/* Insere na fila (FIFO) */
int cq_enqueue(CircularQueue *q, int elem);
/* Remove da fila (FIFO) */
int cq_dequeue(CircularQueue *q, int *out);
/* Olha  o elemento do final da fila */
int cq_peek(const CircularQueue *q, int *out);
/* Verifica se está vazia */
int cq_isEmpty(const CircularQueue *q);
/* Verifica se está cheia*/
int cq_isFull(const CircularQueue *q);
/* Retorna o tamanho da fila */
int cq_size(const CircularQueue *q);