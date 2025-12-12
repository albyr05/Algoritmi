#ifndef PQ_H
#define PQ_H

#include "item.h"

typedef struct pqueue *PQ;

PQ pq_init(int maxN);
void pq_free(PQ *pq);
int pq_is_empty (PQ *pq);
void pq_insert (PQ *pq, Item val);
Item pq_extract_max (PQ *pq);
void pq_display (PQ *pq);
int pq_size (PQ *pq);
void pq_change (PQ *pq, Item val);




#endif