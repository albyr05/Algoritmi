#include<stdio.h>
#include<stdlib.h>
#include "act.h"
#include "ST.h"
#include <string.h>

#define FILENAME "input.txt"
typedef enum{false, true} bool;
typedef struct act activity;


struct act {
    char name[20];
    int start;
    int lasting;
    int profit;
    int n_prio;
    int *prio;
};


struct activities{
    activity *act_v;
    int N;
    ST st; // realizzo un hashmap che dato un nome di una attività mi restituisca l'indice nel vettore act_v;
};


ACT readfile (char *filename){
    FILE *fp = fopen(filename, "r");
    int NA, NP;
    int i;
    ACT act = activities_init(NA);
    char act_name[20];
    for (i = 0; i < NA; i++){
        fscanf(fp, "%s", act_name);
        STinsert(act->st, act_name, i);
        int index = STsearch(act->st, act_name);
        fscanf (fp, "%d %d %d", &act->act_v[index].start, &act->act_v[index].lasting, &act->act_v[index].profit);
        strcpy(act->act_v[index].name, act_name);
        act->act_v[index].n_prio = 0;       // inizializzo tutto senza priorità
        act->act_v[index].prio = NULL;
    }
    char id_prio[20];
    for (i = 0; i < NP; i++){
        fscanf (fp, "%s", id_prio);
        int index = STsearch(act->st, id_prio);
        int c;
        int count = 0;
        char possible_prio[3][20];
        while ((c = getc(fp)) != '\n' && c != EOF){
            fscanf (fp, "%s", possible_prio[count]);
            count ++;
        }
        act->act_v[index].n_prio = count;
        act->act_v[index].prio = malloc(count *sizeof(int));
        int j, k;
        for (j = 0; j < count; j++){
            k = STsearch(act->st, possible_prio[j]);
            act->act_v[index].prio[j] = k; // mi salvo l'indice dell'attività che deve essere terminata per forza;
        }
    }
    fclose(fp);
    return act;
}

bool check_compatible(activity a, activity b){
    if (a.start + a.lasting < b.start) return true;
    return false;
}

bool checkSelection(ACT a, char **selected, int nsel){
    int i, j;
    for (i = 0; i < nsel; i++){
        int index = STsearch(a->st, selected[i]);
        for (j = 0; j < i; j++){
            int index_check = STsearch(a->st, selected[j]);
            if (!check_compatible(a->act_v[index_check], a->act_v[index])) return false;
        }
        bool check_priority = false;
        if (a->act_v[index].n_prio != 0){
            for (int k = 0; k < a->act_v[index].n_prio; k++){
                check_priority = false;
                for (j = 0; j < i; j++){
                    int index_check = STsearch(a->st, selected[j]);
                    if (a->act_v[index].prio[k] == index_check) check_priority = true;
                }
                if (!check_priority) return false;
            }
        }
    }
    return true;
}


void find_best_sol(ACT a, int *sol, int pos, int curr_profit, int *best_profit, int *best_sol, int *bestlen, bool *mark){
    if (curr_profit > *best_profit){
        for (int i; i < pos; i++){
            best_sol[i] = sol[i];
        }
        *bestlen = pos;
        *best_profit = curr_profit;
    }

    if (pos >= a->N) return;

    int i;
    for (i = 0; i < a->N; i++){
        if (mark[i] == false){
            bool can_add = true;
            for (int j = 0; j < pos; j++){
                if (!check_compatible(a->act_v[sol[j]], a->act_v[i])) can_add = false;
            }
            if (can_add){

                bool check_on_priority = true;
                for (int k = 0; k < a->act_v[i].n_prio; k++){       // ciclo che viene balzato se non ci sono priorità
                    bool priority_completed = false;
                    for (int j = 0; j < pos; j++){
                        if (sol[j] == k) priority_completed = true;
                    }
                    if (!priority_completed) check_on_priority = false;
                }

                if (check_on_priority){
                    sol[pos] = i;
                    mark[i] = true;
                    find_best_sol(a, sol, pos+1, curr_profit + a->act_v[i].profit, best_profit, best_sol, best_sol, mark);
                    mark[i] = false;
                }
            }
        }
    }
    return;
}


char ** find_best_sol_wrapper(ACT a){
    int *sol = malloc(a->N*sizeof(int));
    int *best_sol = malloc(a->N*sizeof(int));
    int best_profit = 0, best_len = 0;
    bool *mark = calloc(a->N, sizeof(int));
    find_best_sol(a, sol, 0, 0, &best_profit, best_sol, &best_len, mark);
    char **best_selection = malloc(best_len*sizeof(char*));

    for (int i = 0; i < best_len; i++){
        best_selection[i] = strdup(STsearchByIndex(a->st, best_sol[i]));
    }

    printf("MAX PROFIT %d", best_profit);

    free(best_sol); free(sol);
    return best_selection;
}

int main(int argc, char* argv[]){

    ACT a = readfile(FILENAME);
    char **selection = find_best_sol_wrapper(a);
    free_act (a);
    printf("\n");
    return 0;
}