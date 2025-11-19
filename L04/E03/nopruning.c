#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

// REALLY BAD SOLTION IMPLEMENTATION: WORKS ONLY ON VERY SMALL SETS
bool check_valid_sol(int *sol, int *stones_N, int k){
    int i;
    int *check_dim = calloc(4, sizeof(int));
    if (check_dim == NULL) {
        printf("ERRORE ALLOCAZIONE\n");
        return false;
    }
    
    // Conta le pietre usate
    for(i = 0; i < k; i++){
        check_dim[sol[i]]++;
    }
    
    // Verifica che non si usino più pietre del disponibile
    for (i = 0; i < 4; i++){
        if (check_dim[i] > stones_N[i]) {
            free(check_dim); 
            return false;
        }
    }
    
    // Verifica le transizioni
    for(i = 0; i < k-1; i++){
        // CORREZIONE: sol[1] -> sol[i]
        if ((sol[i] == 0 || sol[i] == 2) && !(sol[i+1] == 0 || sol[i+1] == 1)){
            free(check_dim);
            return false;
        }
        else if ((sol[i] == 1 || sol[i] == 3) && !(sol[i+1] == 3 || sol[i+1] == 2)){
            free(check_dim);
            return false;
        }
    }
    
    free(check_dim);
    return true;
}

int calculate_sol(int pos, int *val, int *sol, int n, int k, int cnt, int *stones_N){
    int i;
    
    if (cnt >= 1){
        return cnt;
    }
    
    if (pos >= k){
        if (check_valid_sol(sol, stones_N, k)){
            for (i = 0; i < k; i++){
                printf("%d ", sol[i]);
            }
            return 1;
        }
        return 0;
    }
    
    for (i = 0; i < n; i++){
        sol[pos] = val[i];
        cnt = calculate_sol(pos+1, val, sol, n, k, cnt, stones_N);
        if (cnt >= 1) return cnt; // Ottimizzazione: esci appena trovi una soluzione
    }
    return cnt;
}

int create_stones_set(int *stones_N){
    int N = 0, i;
    for (i = 0; i < 4; i++){
        N += stones_N[i];
    }
    
    int val[4] = {0, 1, 2, 3};
    int k;
    int cnt = 0;
    
    // Prova dalla lunghezza massima alla minima
    for (k = N; k > 0; k--){
        int *sol = malloc(k * sizeof(int));
        if (sol == NULL) {
            printf("\n\nERRORE MEMORIA\n");
            return 0;
        }
        
        cnt = calculate_sol(0, val, sol, 4, k, 0, stones_N);
        free(sol);
        
        if (cnt >= 1){
            return k; // Trovata la lunghezza massima
        }
    }
    
    return 0;
}

void readfile(FILE *fp){
    int nr, i, j, k;
    int *stones_N = malloc(4 * sizeof(int));
    
    if (stones_N == NULL){
        printf("ERRORE ALLOCAZIONE\n");
        return;
    }
    
    fscanf(fp, "%d", &nr);
    
    for (i = 0; i < nr; i++){
        for (j = 0; j < 4; j++){
            fscanf(fp, "%d", &stones_N[j]);
        }
        printf("SOLUZIONE OTTIMALE RIGA %d:     ", i);
        k = create_stones_set(stones_N);
        printf("    len(%d)", k);
        printf("\n");
    }
    
    free(stones_N);
    fclose(fp);
}

int main(){
    FILE *fp = fopen("easytest.txt", "r");
    
    if (fp == NULL){
        printf("ERRORE: impossibile aprire il file\n");
        return 1;
    }
    
    readfile(fp);
    printf("\n\nPROGRAMMA TERMINATO\n");
    return 0;
}