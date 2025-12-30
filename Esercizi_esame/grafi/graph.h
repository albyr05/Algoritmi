#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph *Graph;

typedef enum {false, true} bool;
Graph read_file(char *filename);
void print_graph(Graph G);
void graph_free(Graph G);


void graph_path(Graph G, int id1, int id2);
void dfs_w(Graph G, int id1);
void graphcc(Graph G);
void dagrts(Graph G);
void graphscc(Graph G);

#endif