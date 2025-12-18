#ifndef GRAPH_H
#define GRAPH_H


typedef struct graph *Graph;
typedef struct simbletable *ST;
typedef struct ladj *l;
typedef enum {false, true} bool;
typedef enum{check_matrix, check_ladj} option;

Graph readfile(char *filename);
void print_order_wrap (Graph G);
void create_ladj_w(Graph G);
bool check_adj_w (char *label1, char *label2, char *label3, Graph G, option o);

#endif