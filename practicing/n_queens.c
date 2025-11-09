#include<stdio.h>
#include<stdlib.h>


typedef enum{false, true} bool;

void alloc_mat(int ***matr){
    int **m = malloc(8*sizeof(int*));
    for (int i = 0; i < 8; i ++){
        m[i] = calloc(8, sizeof(int));
    }
    *matr = m;
}

void stampa_matrice(int **M, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++)
            printf("%d	", M[i][j]);
        printf("\n");
    }
}

bool check_valid(int **m, int riga, int colonna, int N){
    // 1. Controllo Colonna (perché riga è implicita se usiamo riga+1)
    for (int i = 0; i < riga; i++){ // Controlla solo le righe precedenti
        if (m[i][colonna] == 1) return false;
    }
    
    // 2. Controllo Diagonale Principale (top-left a bottom-right)
    int i = riga - 1;
    int j = colonna - 1;
    while (i >= 0 && j >= 0){
        if (m[i][j] == 1) return false;
        i--; j--;
    }
    
    // 3. Controllo Diagonale Secondaria (top-right a bottom-left)
    i = riga - 1;
    j = colonna + 1;
    while (i >= 0 && j < N){
        if (m[i][j] == 1) return false;
        i--; j++;
    }
    
    return true;
}

bool place_queen(int **m, int i, int j){
    if (i == 8){
        stampa_matrice(m, 8, 8);
        return true;
    }
    for (j = 0; j < 8; j++){
        if (check_valid(m, i, j, 8)){
            m[i][j] = 1;
            if (place_queen(m, i+1, j)) return true;
            m[i][j] = 0;
        }
        
    }
    return false;
}

int main(int argc, char* argv[]){
    int **m;
    alloc_mat(&m);
    place_queen(m, 0 , 0);
    printf("\n");
    return 0;
}
