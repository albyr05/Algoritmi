#include<stdio.h>
#include<stdlib.h>
#include "grafo.h"
#include "UF.h"

typedef enum{false, true} bool;

typedef struct{
    int v, u, w;
}edge;

struct Grafo{
    int V;
    int *vertici;
    edge* edges;
    int **madj;
};

graph graph_init(int V){
    graph G = malloc(sizeof(*G));
    G->V = V;
    G->vertici = malloc(V*sizeof(int));
    G->edges = malloc(V*V*sizeof(int));
    G->madj = malloc(V*sizeof(int*));
    for (int i = 0; i< V; i++){
        G->madj[i] = calloc(V, sizeof(int));
    }
    return G;
}

int insert_vertex (int v, int u, int l, int *vect){
    int i;
    bool flag1 = false, flag2 = false;
    for (i = 0; i < l; i++){
        if (vect[i] == v) flag1 = true;
        if (vect[i] == u) flag2 = true;
    }
    if (!flag1) vect[l++] = v;
    if (!flag2) vect[l++] = u;
    return l;
}


graph readfile (char *filename){
    FILE *fp = fopen("grafo.txt", "r");
    int V;
    fscanf(fp,"%d", &V);
    graph G = graph_init(V);
    int i = 0; int l = 0;
    int v, u, w;
    while(fscanf(fp, "%d %d %d", &v, &u, &w) == 3){
        G->edges[i++] = (edge) {v, u, w}; 
        l = insert_vertex (v, u, l, G->vertici);
        G->madj[v][u] = w;
        G->madj[u][v] = w;
    }
    return G;
}

bool check_miniG(graph miniG, graph G, int *check_vertex, int K){ 
    int i, cnt = 0;

    if (!dfs(miniG)) return false;
    bool rep = false;
    for (i = 0; i < G->V^2; i++){
        if (check_vertex[miniG->edges[i].v] == 1 || check_vertex[miniG->edges[i].u] == 1) rep = true;
        check_vertex[miniG->edges[i].v] == 1;
        check_vertex[miniG->edges[i].u] == 1;
        cnt++;
    } 
    if (!rep) return false;

    if (cnt < K) return false;

    return true;
}



bool check_proposal(char *filename, graph G, int K){
    FILE *fp = fopen("proposta.txt", "r");
    int N, i ;
    fscanf (fp, "%d", &N);// numero di sottogruppi di E
    int v, u, w;
    int *check_vertex = calloc(G->V, sizeof(int));
    
    for (i = 0; i < N; i++){
        int l = 0;
        graph miniG = graph_init(G->V);
        while (fscanf(fp, "%d %d", &v, &u, &w) == 3){
            miniG->edges[l++] = (edge) {v, u, w};
        }   
        if (!(check_miniG(miniG, G, check_vertex, K))) return false;
    }
    for (int j = 0; j < G->V; j++){
        if(check_vertex = 0) return false;
    }
    return true;
}

int calculateMST(int *part, int curr_par, edge *sorted_edges, int V, int E){
    int n_vertici = 0;
    int n_edges = 0;
    int weight = 0;
    UF uf = UFinit(V);
    int i;
    for (i = 0; i < V; i++){
        if (part[i] == curr_par) n_vertici ++;
    }

    if (n_vertici == 1 || n_vertici == 0) return 0;

    for (i = 0; i < E; i++){
        int u = sorted_edges[i].u;
        int v = sorted_edges[i].v;
        if (part[u] == curr_par && part[v] == curr_par){
            if (UFifnd (uf, u) != UFfind(uf, v)){
                UFunion (uf, u, v);
                weight += sorted_edges[i].w;
                n_edges ++;
            }
        }
    }

    if (n_edges == n_vertici-1) return weight;
    else return -1;
}

void gen_k_partition (int pos, int curr_part, int *part, int V, int *best_weight, int K, edge *sorted_edges, int E){
    if (pos == V){
        int tot_weight = 0;
        int i;
        int *counts = calloc(curr_part, sizeof(int));
        for (i = 0; i < pos; i++){
            counts[part[i]] ++;
        }
        for (i = 0; i < curr_part; i++){
            if (counts[i] < K) return;
        }
        for (i = 0; i < curr_part; i++){
            int weight = calculateMST(part, i, sorted_edges, V, E);
            if (weight == -1) return;
            tot_weight += weight;
        }

        if (tot_weight < *best_weight){
            *best_weight = tot_weight;
        }
        return;

    }

    int i;
    for (i = 0; i < curr_part; i++){
        part[pos] = i;
        gen_k_partition(pos+1, curr_part, part, V, best_weight, K, sorted_edges, E);
    }

    part[pos] = curr_part;
    gen_k_partition(pos+1, curr_part+1, part, V, best_weight, K, sorted_edges, E);
}


