#ifndef GRAPH_H
#define GRAPH_H

#include "ST.h"

typedef struct graph *GRAPH;
typedef enum {false, true} bool;

// Funzioni di gestione del grafo
GRAPH graph_init(int V);
GRAPH graph_load(char *filename);
void graph_free(GRAPH G);

// Funzioni richieste richiamabili nel main
bool relationshipK(GRAPH G, char *name1, Date d1, char *name2, Date d2, int k);
void largestLineageK(GRAPH G, int k);

#endif
