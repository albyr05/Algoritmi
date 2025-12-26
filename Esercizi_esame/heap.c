#include<stdio.h>
#include<stdlib.h>
#include "heap.h"

typedef enum{false, true} bool;

struct Heap{
    int heap_size;
    int *v;
};

h heap_init(int maxn){
    h heap = malloc(sizeof(*heap));
    heap->v = malloc (maxn*sizeof(int));
    heap->heap_size = maxn;
    return heap;
}

void heap_read (h heap, int n){
    int i;
    for (i = 0; i < n; i++){
        scanf(" %d", &heap->v[i]);
    }
    return;
}
int LEFT (int i){return i*2 +1;}
int RIGHT (int i){return i*2 +2;}
int PARENT (int i) {return (i-1)/2;}

void swap(h heap, int i, int j){
    int temp = heap->v[i];
    heap->v[i] = heap->v[j];
    heap->v[j] = temp;
}

void heapify(int i, h heap){
    int largest = i;
    if (heap->v[LEFT(i)] > heap->v[largest] && LEFT(i)< heap->heap_size) largest = LEFT(i); 
    if (heap->v[RIGHT(i)] > heap->v[largest] && RIGHT(i) < heap->heap_size) largest = RIGHT(i);
    if (i != largest){
        swap(heap, largest, i);
        heapify(largest, heap);
    } 
    return;
}

void heapbuild(h heap){
    int i;
    for (i = (heap->heap_size)/2 - 1; i >= 0; i--){
        heapify(i, heap);
    }
    return;
}

void print_heap(h heap){
    int i;
    for (i = 0; i < heap->heap_size; i++){
        printf("%d ", heap->v[i]);
    }
    return;
}

void heap_sort(h heap){
    int oldsize = heap->heap_size;
    int i;
    for (i = heap->heap_size-1; i > 0; i --){
        swap(heap, 0, i);
        heap->heap_size --;
        heapify(0, heap);
        printf("\nSTEP %d: " , oldsize -i);
        print_heap(heap);
        if (oldsize-i == 2) break;
        }
    heap->heap_size = oldsize;
    return;
}
