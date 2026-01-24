#include<stdio.h>
#include<stdlib.h>
#include "graph.h"

typedef struct node *link;


typedef enum{false, true} bool;

struct node{
    int w;
    link next;
};

struct graph{
    int V;
    int *v;
    link *ladj;
    int **madj;
};

GRAPH graph_read (char *filename){
    FILE *fp = fopen(filename, "r");
    int V;
    fscanf (fp, "%d", &V);
    GRAPH G = graph_init(V);
    int u, w;
    while (fscanf (fp, "%d %d", &u, &w) == 2 ){
        G->v[u] = u;
        G->v[w] = w;
        insert_node (G->ladj[u], w);
        insert_node (G->ladj[w], u);
    }
    return G;
}

// il triangle packing è una partizione dei vertici 
// il valore i-esimo del vettore partition indica a che a che gruppo appartiene il vertice i;
// -1 per i vertici esclusi da qualsiasi triangle packing
bool check_valid_triangle_packing(GRAPH G, int *partition){
    int i, j;
    bool *visited = calloc(G->V, sizeof(int));
    for (i = 0; i < G->V; i++){
        if (partition[i] != -1 && visited[i] != 0){
            int curr_pack = partition[i];
            int count = 1;
            for (j = i; j < G->V; j++){
                if (partition[j] == curr_pack){
                    visited[j] = 1;
                    link t;
                    for (t = G->ladj[i]; t != NULL; t = t->next){
                        if (t->w == j){
                            count ++;
                            break;
                        }
                    }
                }
            }
            if (count != 3) {
                free(visited);
                return false;
            }
        }
    }
    free(visited);
    return true;
}

bool check (int *sol, int id, int i, GRAPH G, int current_insert){
    int j;
    int count = 1;
    for (j = 0; j < G->V; j++){
        if (sol[j] == id && G->madj[i][j] == 1){
            count++;
        }
    }
    if (current_insert == count) return true;
    return false;
}

bool calc_sol(int pos, GRAPH G, int *sol, int k, int *taken, int *triangoli_completati){
    if (*triangoli_completati == k){
        return true;
    }

    if (pos == G->V){
        return false;
    }

    sol[pos] = -1;
    if (calc_sol(pos+1, G, sol, k, taken, triangoli_completati)) return true;

    
    int i;
    bool can_add = false;
    for (i = 0; i < k; i++){
        if(taken[i] >= 3) continue;

        if (taken[i] != 0){
            if (check(sol, i, pos, G, taken[i])){
                sol[pos] = i;
            }
            if (taken[i] == 3) {
                triangoli_completati += 1;

            }
        else {
            sol[pos] = i;
        }

            taken[i]++;
            calc_sol(pos+1, G, sol, k, taken, triangoli_completati);
            taken[i]--;
        }
    }
}
void wrapper (GRAPH G){
    int *sol = malloc(G->V*sizeof(int));
    int i;
    for (i = 0; i < G->V; i++){
        sol[i] = -1;
    }

    int k;
    for (k = G->V/3; k > -1; k--){
        int taken = calloc(k, sizeof(int));
        calc_sol(0, G, sol, k, taken, 0);
        free(taken);
    }
}

int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}