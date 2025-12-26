#ifndef HEAP_H
#define HEAP_H
typedef struct Heap *h;
void heapbuild (h heap);
void heap_sort(h heap);
void print_heap(h heap);
h heap_init(int maxn);
void heap_read (h heap, int n);

#endif