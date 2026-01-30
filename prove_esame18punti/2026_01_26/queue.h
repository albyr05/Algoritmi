#ifndef QUEUE_H

#define QUEUE_H


typedef struct queue *Q;


Q Q_init(int V);
int Q_empty(Q queue);
void Q_insert(Q queue, int id);
int Q_extract (Q queue);
void Q_free(Q queue);


#endif

