#include<stdio.h>
#include<stdlib.h>
#include "pq.h"
typedef enum{false, true} bool;

struct pqueue {
    int heap_size;
    int *v;
};

int left (int i){return i*2 +1;}
int right (int i){return i*2 +2;}
int parent (int i) {return (i-1)/2;}

void SWAP(PQ pq, int i, int j){
    int temp = pq->v[i];
    pq->v[i] = pq->v[j];
    pq->v[j] = temp;
}

PQ pq_init(int maxN){
    PQ pq = malloc(sizeof(*pq));
    pq->v = malloc(maxN*sizeof(int));
    pq->heap_size = 0;
    return pq;
}

void queueify(int i, PQ pq){
    int smaller = i;
    if (pq->v[left(i)] < pq->v[smaller] && left(i)< pq->heap_size) smaller = left(i); 
    if (pq->v[right(i)] < pq->v[smaller] && right(i) < pq->heap_size) smaller = right(i);
    if (i != smaller){
        SWAP(pq, smaller, i);
        queueify(smaller, pq);
    } 
    return;
}
void pq_insert(PQ pq, int val){
    pq->heap_size ++;
    int i = pq->heap_size-1;
    while (i >= 1 && val < pq->v[parent(i)]){
        pq->v[i] = pq->v[parent(i)];
        i = parent(i);
    }
    pq->v[i] = val;
    return;
}

int pq_extract_max (PQ pq){
    SWAP(pq, 0, pq->heap_size - 1);
    int val_max = pq->v[pq->heap_size-1];
    pq->heap_size --;
    queueify(0, pq);
    return val_max;
}

void pq_change(PQ pq, int new_val, int old_val){
    int i, pos;
    bool found = false;
    for (i = 0; i < pq->heap_size && !found; i++){
        if (pq->v[i] == old_val){
            pos = i;
            found = true;
        }
    }
    if (found){
        while(pos >= 1 && new_val < pq->v[parent(pos)]){
            pq->v[pos] = pq->v[parent(pos)];
            pos = parent(pos);
        }
        pq->v[pos] = new_val;

        queueify(0, pq);
    }
    return;

}

void pq_display (PQ pq){
    for (int i = 0; i < pq->heap_size; i++){
        printf("%d ", pq->v[i]);
    }
}