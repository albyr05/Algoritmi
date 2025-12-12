#include<stdio.h>
#include<stdlib.h>
#include "heap.h"


struct heap_s {
    int *A;
    int heap_size;
};

int LEFT (int i){return i*2+1;}
int RIGHT (int i) {return i*2+2;}
int PARENT (int i) {return (i-1)/2;}

Heap heap_init(int maxn){
    Heap h = malloc(sizeof(*h));
    h->A = malloc (maxn*sizeof(int));
    h->heap_size = 0;
    return h;
}

void heap_free(Heap h){
    free(h->A); free(h);
}

void heap_fill(Heap h, int val){
    int i; 
    i = h->heap_size++;
    h->A[i] = val;
    return;
}

void heap_display(Heap h){
    int i;
    for (i = 0; i < h->heap_size; i++){
        printf("%d ", h->A[i]);
    }
    return;
}
// should implement also the KEYGET but stupid here with int items 
int KEYCMP (int x, int y){
    if (x >= y) return 1;
    return -1;
}
void SWAP(Heap h, int j, int k){
    int tmp = h->A[j];
    h->A[j] = h->A[k];
    h->A[k] = tmp; 
}


// O(log(N))
void heapify (Heap h, int i){
    int l, r, major;
    l = LEFT(i);
    r = RIGHT(i);
    if (l < h->heap_size && KEYCMP(h->A[l], h->A[i]) == 1) major = l;   // looking for the max between the 3 
    else major = i;     
    if (r < h->heap_size && KEYCMP(h->A[r], h->A[major]) == 1) major = r; // comparing right son with the current major
    if (major != i){
        SWAP(h, i, major);
        heapify(h, major);
    }
    return;
}

// O(N) used to tranform any array into an heap
void heap_build(Heap h){
    int i;
    for (i = (h->heap_size)/2-1; i >= 0; i--){      // apply heapify from leaf to root
        heapify(h, i);
    }
}

// O(N*log(N)) sorting algortithm
void heap_sort(Heap h){
    int i, j;
    heap_build(h);
    j = h->heap_size;
    for (i = h->heap_size-1; i >0; i--){
        SWAP(h, 0, i);      // swapping the first element with the last 
        h->heap_size --;
        heapify(h, 0);      // heapifying the tree
    }
    h->heap_size = j;   // restoring the initial size
}