#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
typedef struct {
    int start;
    int end;
}activity;

void alloc(int n, activity **vect){
    activity *v = malloc(n*sizeof(activity));
    *vect = v;
}

void free_memory (activity *v){
    free (v);
}

// function used to know if 2 activity are compatibile
bool check_compatibility(activity a, activity b){
    if (a.start <b.end && b.start < a.end) return false;
    return true; 
}

void max_subests(int pos, activity *sol, int *max_d, int current_d, int N, activity *v, activity *best_sol, int *len, int start){
    int i;
    if (current_d > *max_d) {       
        *max_d = current_d;     // updating the max durating value 
        for (i = 0; i < pos; i++){
            best_sol[i] = sol[i];
        }
        *len = pos;
    }

    if (pos >= N) return;
    
    for (i = start; i < N; i++){
        sol[pos] = v[i];
        bool compatibile = true;
        for (int j = 0; j < pos; j++) {
            if (check_compatibility(sol[j], v[i]) == false) {
                compatibile = false;
                break;
            }
        }
        if (compatibile) {       // if the activity is compatibile with all the previous i can proceed with recursion
            int d = v[i].end - v[i].start;      
            current_d += d;         // adding the duration of the just insert activity
            max_subests(pos+1, sol, max_d, current_d, N, v, best_sol, len, i + 1);      // proceed with recursion
            current_d -= d;         // backtracking --> removes the activity 
        }   
    }
    return ;
}

void actsel(int N, activity *v){
    activity *sol, *best_sol;
    int len = 0, pos = 0, current_d = 0, max_d = 0, i;
    alloc(N, &sol);     // allocating the vectors of solution 
    alloc(N, &best_sol);
    max_subests(pos, sol, &max_d, current_d, N, v, best_sol, &len, 0);      // calling the recursive func 
    printf("{");
    for (i = 0; i < len; i++){  
        printf("(%d, %d) ", best_sol[i].start, best_sol[i].end);        // printing the last first best sol
    }
    printf("}");
    printf("    max last %d", max_d);        // printing also the max last 
    free_memory(sol);       // freeing memory
    free_memory(best_sol);
    return;
}

void readfile(){
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL){
        printf("Errore nell'apertura del file");
        exit(-1);
    }
    int N, i;
    fscanf(fp, "%d", &N);
    activity *v = malloc(N*sizeof(activity));       // allocating the base struct vector
    for (i = 0; i < N; i++){
        fscanf(fp, "%d %d", &v[i].start, &v[i].end);
    }
    actsel(N, v);
    fclose(fp);
    return;
}


int main(int argc, char* argv[]){
    readfile();
    printf("\nPROGRAMMA TERMINATO");
    return 0;
}