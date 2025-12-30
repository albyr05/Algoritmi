#ifndef PQ_H
#define PQ_H
typedef struct pqueue *PQ;

PQ pq_init(int maxN);

void pq_insert (PQ pq, int val);
int pq_extract_max (PQ pq);
void pq_display (PQ pq);
void pq_change (PQ pq, int new_val, int old_val);




#endif