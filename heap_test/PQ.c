#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "item.h"
#include "PQ.h"


typedef enum{false, true} bool;

struct pqueue {
    Item *A; int heapsize;
};

int LEFT (int i){return i*2+1;}
int RIGHT (int i) {return i*2+2;}
int PARENT (int i) {return (i-1)/2;}

void SWAP(PQ pq, int j, int k){
    Item tmp = pq->A[j];
    pq->A[j] = pq->A[k];
    pq->A[k] = tmp; 
}


PQ pq_init (int maxN){
    PQ pq = malloc(sizeof(*pq));
    pq->A = malloc(maxN*sizeof(Item));
    pq->heapsize = 0;
    return pq;
}

void pq_free(PQ pq){ free(pq->A); free(pq);}

int pq_size(PQ pq) {return (pq->heapsize == 0);}

void pq_display(PQ pq){
    for (int i = 0; i < pq->heapsize; i++){
        print_item(pq->A[i]);
    }
}

void pq_insert (PQ pq, Item val){
    int i = pq->heapsize++;
    while (i >= 1 && KEYcmp(pq->A[i], pq->A[PARENT(i)]) > 0){
        pq->A[i] = pq->A[PARENT(i)];
        i = PARENT(i);
    }
    pq->A[i] = val; 
    return;
}

void heapify (PQ pq, int i){
    int r = RIGHT(i), l = LEFT(i);
    int major = i;
    if (KEYcmp(pq->A[r], pq->A[major])) major = r;
    if (KEYcmp(pq->A[l], pq->A[major])) major = l;
    if (major != i){
        SWAP(pq, i, major);
        heapify(pq, major);

    }
}

Item pq_extract_max(PQ pq){
    Item maxprio;
    SWAP(pq, 0, pq->heapsize-1);
    maxprio = pq->A[pq->heapsize-1];
    pq->heapsize--;
    heapify(pq, 0);
    return maxprio;
}

// cambia la priority di un elemento della coda e riordina con heapify O(n)
void pq_change(PQ pq, Item val){
    int i, pos;
    bool found = false;
    for (i = 0; i < pq->heapsize && !found; i++){       // finding the item by namme 
        if (strcmp(pq->A[i].name, val.name) == 0) pos = i; found = true;        // saving the pos 
    }

    if (found){
        while (pos >= 1 && KEYcmp(val, pq->A[PARENT(pos)])){    // inserting the new item up (cf pqinsert) finding the correct pos checking the priority
            pq->A[pos] = pq->A[PARENT(pos)];
            pos = PARENT(pos);
        }
        pq->A[pos] = val;
        heapify(pq, pos);       // fixing the new item down with heapify
    }
    else printf("item not found\n");
}