#include<stdio.h>
#include<stdlib.h>
#include "ST.h"
#include "struct.h"

typedef struct node *link;
typedef enum {false, true} bool;

struct node{
    int w;
    link next;
};

typedef struct{
    int start;
    int end;
    int cost;
} act;

struct activities{
    int N;
    act *el;
    ST st;
    link *ladj;
};
//t->v (arco destinazione)
//if (a.stop > t->v.start)

struct solution{
    int *partition;
    int *CP;        // inizializzo a zero e salvo i compensi della i-esima persona
    int optimal_value;
};

bool check_if_possible(ACT a){
    int i, j;
    for (i = 0; i < a->N; i++){
        link t;
        for (t = a->ladj[i]; t != NULL; t = t->next){
            if(a->el[i].start < a->el[t->w].end) {
                return false;
            }
        }
    }
    return true;
}

void dfs (link *ladj, int i, int *visited, int *has_cycle){
    visited[i] = 1;

    for (link t = ladj[i]; t != NULL; t = t->next){
        if (visited[t->w] == 1){
            *has_cycle = true;
        }
        if (visited[i] == 0){
            dfs(ladj, t->w, visited, has_cycle);
        }
    }
    visited[i] = 2;
}

bool dfs_wrap(ACT a){
    int *visited = calloc(a->N,sizeof(int));
    int has_cycle = false;
    for (int i = 0; i < a->N; i++){
        if (visited[i] == 0){
            dfs (a->ladj, i, visited, &has_cycle);
        }
    }
    return (has_cycle);
}

void print_sink_source(ACT a){
    bool has_cycle = dfs_wrap (a);

    if (has_cycle) {
        printf("il grafo non è un DAG e nemmeno un albero");
        return;
    }

    int *in_degree = calloc(a->N, sizeof(int));
    int *out_degree = callc(a->N, sizeof (int));
    int i;

    for (i = 0; i < a->N; i++){
        link t;
        for (t = a->ladj[i]; t != NULL; t++){
            out_degree[i] ++;
            in_degree[t->w] ++;
        }
    }
    int count = 0;
    for (i = 0; i < a->N; i++){
        if (in_degree[i] > 1){
            count ++;
        }
    }

    if (count > 1){
        printf("il grafo è un albero");
    }
    else {
        printf ("il grafo è un DAG");
    }

    for (i = 0; i < a->N; i++){
        if (out_degree[i] == 0){
            char name[20] = STsearchByIndex(a->st, i);
            printf("L'attività %s è un sink/foglia");
        }

        if (in_degree[i] == 0){
            char name[20] = STsearchByIndex(a->st, i);
            printf("L'attività %s è un source/radice");
        }
    }

    return;

}

bool sovrapposition(act a, act b){
    if (b.start < a.end && a.start < b.end ) return true;
    return false;
}

int ACT_min_pers(ACT a){
    int max = 0;
    int i, j;
    for (i = 0; i < a->N; i++){
        int active_together = 1;
        for (j = 0; j < i; j++){
            if (sovrapposition(a->el[j], a->el[i])) active_together ++;
        }
        if (active_together > max){
            max = active_together;
        }
    }
    return max;
}

bool can_add(ACT a, int pos, int id_pers, int *partition ){
    int i;
    for (i = 0; i < a->N; i++){
        if (partition[i] == id_pers){
            if (sovrapposition(a->el[pos], a->el[i])){
                return false;
            }
        }
    }
    return true;
}


void evaluate_sol(SOL curr_sol, int N_pers){
    int sum = 0, i;
    for (i = 0; i < N_pers; i++){
        sum += curr_sol->CP[i];
    }
    int cp_avg = sum /N_pers;

    int val = 0;
    for (i = 0; i < N_pers; i++){
        val += abs(curr_sol->CP[i] - cp_avg);
    }

    curr_sol->optimal_value = val;

    return;
}


void calc_best_partition (int pos, ACT a, SOL curr_sol, SOL best_sol, int N_pers){
    int i;
    if (pos == a->N){
        evaluate_sol(curr_sol, N_pers);
        if (curr_sol->optimal_value < best_sol->optimal_value){
            copysol(curr_sol, best_sol);
        }
        return ;
    }

    
    
    for (i = 0; i < N_pers; i++){
        bool is_valid = false;
        
        if (curr_sol->CP[i] == 0){
            is_valid = true;
        }

        else if (can_add(a, pos, i, curr_sol->partition)){
            is_valid = true;
        }

        if (is_valid){
            curr_sol->partition[pos] = i;
            curr_sol->CP[i] += a->el[pos].cost;
            calc_best_partition(pos+1, a, curr_sol, best_sol, N_pers);
    
            curr_sol->CP[i] -= a->el[pos].cost; // backtracking
        }
        
    }
    return;
}

void ACT_best_plan (ACT a){
    int N_pers = ACT_min_pers(a);
    quick_sort (a->el, a->N);   // ordino le attività per tempo di inizio crescente;
    SOL curr_sol = sol_init(a->N, N_pers);
    SOL best_sol = sol_init(a->N, N_pers);
    best_sol->optimal_value = 99999999;
    calc_best_partition(0, a, curr_sol, best_sol, N_pers);
}


