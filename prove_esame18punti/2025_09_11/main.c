#include<stdio.h>
#include<stdlib.h>
#include "struct.h"
#include "ST.h"
typedef enum{false, true} bool;

#define MAX_S 25

typedef struct node *link;

struct node{
    int w;
    int distance_adress; // è zero per l'adiacenza delle persone vincolate 
    // 0 cmq se abitano nella stessa città, se no vale il tratto comune tra i due bro
    link next;
};


typedef struct {
    char name[MAX_S];
    char adress[MAX_S];
    int N_val;
    int distance_air;
} person;


struct persons{
    person *v;
    int N;
    ST st;
};

struct constrains{
    link *ladj; // salvo i vincoli sulle persone che devono viaggiare assieme
    int n_constrains;
    link *ladj_adresses;        // questa è non orientato
};


struct sol{
    int *part;
    int min_km;
    int N_bus;
};

void dfs (link *ladj, PERSONS P, int i, int *visited, int *counter_people, int *count_val){
    visited[i] = 1; // scoperta del vertice

    (*counter_people) ++;       // aggiungo una persona
    (*count_val) += P->v[i].N_val;      // e le sue relative valigie

    for (link t = ladj[i]; t != NULL; t = t-next){      // prendo tutte quelle che devono viaggiare con lei 
        if (visited[t->w] == 0){        // verice non ancora scoperto
            dfs (ladj, P, t->w, visited, counter_people, count_val);
        }
    }
    visited[i] = 2;     // chiudo il vertice
}


bool is_valild_el (PERSONS P, CONSTRAINS C, int MP, int MV){
    int count_val = 0;
    int counter_people = 0;
    int *visited = calloc(P->N, sizeof(int));

    int i;
    for (i = 0; i < P->N; i++){
        count_val = 0;     // reset dei counters 
        counter_people = 0;
        dfs(C->ladj, P, i, visited, &counter_people, &count_val);
        if(count_val > MV || counter_people > MP){
            free(visited);
            return false;
        }
    }

    free(visited);
    return true;
}


// il modello è un partizionamento, invece che usare l'algoritmo di ER che genererebbe tutte le partizioni conviene utilizzare le dispposizioni ripetute
// l'idea è quella di ciclare nel wrapper la classe delle disposizioni a partire in classi crescenti
// una selezionata una persona in un bus occorre mettere anche tutte quelle vincolate a lei nello stesso
// la condizione di terminazione è quando si sono disposte tutte le persone e poi si valuta l'ottimallità del percorso

int check_if_same_adress_or_common(int i, int j, CONSTRAINS C){
    int k, l;
    link t;
    for (t = C->ladj_adresses[i]; t != NULL; t = t->next){
        if(t->w == j){
            return t->distance_adress;
        }
    }
    return -1;
}

void dfs_take_people (CONSTRAINS C, PERSONS P, int i, SOL current_sol, int id_bus){
    current_sol->min_km += P->v[i].distance_air;
    current_sol->part[i] = id_bus;
    link t;
    for (t = C->ladj[i]; t != NULL; t = t->next){
        if (current_sol->part[t->w] == -1){
            int distance = check_if_same_adress_or_common(i, t->w, C,);
            
            if (distance != -1){
                current_sol->min_km -= P->v[t->w].distance_air;
                current_sol->min_km -= distance; 
            }

            dfs_take_people(C, P, t->w, current_sol, id_bus);
        }
    }
}


void dfs_backtracking (int i, CONSTRAINS C, SOL current_sol){
    current_sol->part[i] = -1;
    link t;
    for (t = C->ladj[i]; t != NULL; t = t->next){
        if (current_sol->part[t->w] != -1){
            dfs_backtracking(t->w, C, current_sol);
        }
    } 
}

bool evaluate_sol (SOL current_sol, int MP, int MV, PERSONS P){
    int i;
    int *check_p = calloc(current_sol->N_bus, sizeof(int));
    int *check_val = calloc(current_sol->N_bus, sizeof(int));
    for (i = 0; i < N; i++){
        check_val[current_sol->part[i]] += P->v[i].N_val;
        check_p[current_sol->part[i]] ++;
    }
    for (i = 0; i < current_sol->part; i++){
        if (check_p[i] > MP) return false;
        if (check_val[i] > MV) return false;
    }
    return true;    

}

void calculate_part (int pos, SOL current_sol, CONSTRAINS C, PERSONS P, SOL best, int *flag, int MP, int MV){
    int i;
    if (pos == P->N){
        if (evaluate_sol(current_sol, MP, MV, P->N)){
            if (current_sol->min_km < best->min_km){
                copy_sol(best, current_sol);
            }
            *flag = true;
        }
        return;
    }

    if (current_sol->min_km > best->min_km) return ;

    if (current_sol->part[pos] == -1){
        for (i = 0; i < current_sol->N_bus; i++){
            int prev_dist = current_sol->min_km;
            dfs_take_people(C, P, pos, current_sol, i); // prendo tutte le persone vincolate e sommo le distanze

            calculate_part(pos+1, current_sol, C, P, best, MP, MV);

            dfs_backtracking(pos, C, current_sol);      // backtracking, tolgo tutte le persone che ho preso 
            current_sol->min_km = prev_dist;            // riporto la distanza a quella che avevo prima di prendere tutte le persone
        }   
        return;
    }

    calculate_part(pos+1, current_sol, C, P, best, MP, MV);
    
    return ;
}



void find_sol_wrap(PERSONS P, CONSTRAINS C){
    bool flag = false;

    for (int k = 0; k < P->N - 1 && !flag; k++){
        SOL sol = sol_init(k);
        SOL best = sol_init(k);
        calculate_part(0, sol, C, P, best, flag);
        free(sol);
        free(best);
    }
}
int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}