#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

typedef struct node *link;

// implemento la coda della BFS con una coda circolare con un indice sia alla coda (inserimento) che estrazione (estrazione)

struct queue{
    int max_size;      // lunghezza massima della coda
    int *q;
    int head;    // indice di estrazione 
    int tail;       // indice di inserimento
    int len;  // indica la lunghezza corrente della coda
};

Q Q_init(int V){
    Q queue = malloc(sizeof(*queue)); // alloco la struct coda circolare
    queue->max_size = V;       // lunghezza massima del vettore
    queue->head = 0;       // puntatore all'estrazione 
    queue->tail = 0;         // puntatore all'inserzione 
    queue->len = 0;            // all'inizio la lunghezza è uguale a 0
    queue->q = malloc(V*sizeof(int));
    return queue;
}


int Q_empty(Q queue){
    if (queue->len == 0) return 1;    // la coda è vuota 
    return 0;
}


void Q_insert(Q queue, int id){
    queue->q[queue->tail] = id;      // inserisco il nuovo vertice 
    queue->tail = (queue->tail + 1 ) % queue->max_size;   // calcolo l'indice della nuova intersezione 
    queue->len++;    // incremento la lunghezza
    return;
}


int Q_extract (Q queue){
    int extracted_val = queue->q[queue->head];
    queue->head = (queue->head+1) % queue->max_size;    // sposto la testa
    queue->len --;   // decremento la lunghezza della coda
    return extracted_val;
}

void Q_free(Q queue){
    free(queue->q);
    free(queue);
    return;
}