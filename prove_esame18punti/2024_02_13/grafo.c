#include<stdio.h>
#include<stdlib.h>
#include "grafo.h"
#include "ST.h"
#include <string.h>

typedef enum{false, true} bool;
typedef struct{
    int v;
    int u;
}edge;

typedef struct {
    char name[20];
    int val;
}vertice;

struct Graph{
    int V;
    int **madj;
    vertice *v;
    ST hash_map;
};

// restituisce il numero univoco di nomi e creo una hash_map che dato un nome mi restituisca un indice 
int get_len_name_set (vertice *v){

}

// restituisce l'indice del nome univoco, del vertice 
int get_index_by_name (char *name, ST hash_map){

}


graph Graph_load(char *filename, int *V, int *E){
    FILE *fp = fopen(filename, "r");

    int i;
    fscanf(fp, "%d %d", V, E);
    graph G = graph_init(V, E);

    int index;
    for (i = 0; i < *V; i++){
        fscanf(fp, "%d");
        fscanf(fp, "%s %d", G->v[i].name, &G->v[i].val);
    }
    for (i = 0; i < *E; i++){
        // inserisco l'arco nella sotto matrice, e nel vettore di archi?
    }
    return G;
}

bool explore_path(int curr_v, char *curr_p, char *string, graph G){
    int j;
    if (*curr_p == '\0'){
        return true;
    }
    for (j = 0; j < G->V; j++){
        if (G->madj[curr_v][j] == 1){
            if (strstr(string, G->v[j].name) == curr_p){
                if (explore_path(j, curr_p + strlen(G->v[j].name), string, G)) return true;
            }
        }
    }
    
    return false; // sono arrivato alla fine dei vertici adiacenti ma non ho trovato nessun match
}

bool check_path (char *string, graph G){
    int start = -1;
    bool found = false ;
    int i;
    char *curr_p = string;
    for (i = 0; i < G->V && !found; i++){
        if (strstr (string, G->v[i].name) == curr_p){
            start = i;
            found = true;
            curr_p += strlen(G->v[i].name);     // vado avanti nella stringa della soluzione della lunghezza della prima parola
        }
    }
    if (start == -1) return false; // non ho trovato un punto di inizio

    if (explore_path(start, curr_p, string, G)) return true;
        

    return false;
    
}

bool compatible (char *a, char *b){
    if (strstr("aeiou", a[strlen(a)-1]) == NULL  && strstr("aeiou", b[strlen(b)-1]) == NULL) return false;
    if (strstr("aeiou", a[strlen(a)-1]) != NULL  && strstr("aeiou", b[strlen(b)-1]) != NULL) return false;
    return true;
}

void build_best_path (int pos, int *sol, graph G, int *best_val, int curr_val, int M, int curr_vert, int *occurencies, int *best_sol, int *best_len){
    if (curr_val > *best_val){
        int i;
        for (i = 0; i < pos; i++){
            best_sol[i] = sol[i];
        }
        (*best_val) = curr_val; 
        *best_len = pos;
        return;
    }

    int j = 0;
    for (j = 0; j < G->V; j++){
        if (G->madj[curr_vert][j] == 1){
            if (compatible(G->v[j].name, G->v[curr_vert].name)){
                int index = get_index_name(G->v[j].name, G->hash_map);
                if (occurencies[index] < M){
                    occurencies[index] ++;
                    sol[pos] = j;
                    build_best_path(pos+1, sol, G, best_val, curr_val + G->v[j].val, M, j, occurencies, best_sol, best_len);
                    occurencies[index] --;
                }
            }
        }
    }
    return ;
}



void build_sol_wrap(graph G, int M){
    int *sol = malloc(G->V *sizeof(int));
    int *best_sol = malloc(G->V *sizeof(int));
    int i;
    int best_val = 0;
    int best_len = 0;
    int occurencies = calloc(LEN_HASHMAP*sizeof(int));
    for(i = 0; i < G-> V; i++){
        sol[0] = i;
        build_best_path(1, sol, G, &best_val, 0, M, i, occurencies, best_sol, &best_len);
    }   

    char string_sol = malloc(20*G->V*sizeof(char));

    for (i = 0; i < best_len; i++){
        string_sol = strcat(string_sol, G->v[i].name);
    }
    printf("SOLUZIONE CAMMINO MASSIMO");
    printf("%s", string_sol);
    printf("\n Valore massimo:    %d", best_val);

    free(sol);
    free(best_sol);
    free(string_sol);

}