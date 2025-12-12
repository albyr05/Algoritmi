#ifndef HEAP_H
#define HEAP_H
typedef struct heap_s *Heap;
Heap heap_init (int maxn);
void heap_free (Heap h);
void heap_fill (Heap h, int val);
void heap_sort (Heap h);
void heap_display (Heap h);
void heapify(Heap h, int i);
void heap_build(Heap h);
void heap_sort (Heap h);
#endif