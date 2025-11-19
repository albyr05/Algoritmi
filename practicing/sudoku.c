#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
void alloc_mat (int ***mat){
    int **m = malloc (9*sizeof(int*));
    for (int i = 0; i < 9; i ++){
        m[i] = malloc (9*sizeof(int));
    }
    *mat = m;
}

void read_file(int **m){
    FILE *fp;
    if ((fp = fopen("sudoku_ex.txt", "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "sudoku_ex.txt");
        exit(-1);
    }
    for (int i = 0; i <9; i++){
        for (int j = 0; j < 9; j++){
            fscanf(fp, "%d ", &m[i][j]);
        }
    }
    fclose(fp);
    return;
}

void stampa_matrice(int **M, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++)
            printf("%d ", M[i][j]);
        printf("\n");
    }
}

bool check (int **m, int cand, int i, int j){
    int k, l;
    for (k = 0; k < 9; k++){
        if (m[i][k] == cand) return false;
    }
    for (k = 0; k < 9; k++){
        if (m[k][j] == cand) return false;
    }
    int start_row = (i / 3) * 3, start_col = (j / 3) * 3;
    for (k = start_row; k < 3; k++){
        for (l = start_col; l < 3; l++){
            if (m[k][l] == cand) return false;
        }
    }
    return true;
}
bool solve(int **m, int i, int j){
    int cand;


    if (m[i][j] != 0){
        if (j != 8){
            if (solve(m, i, j+1)) return true;
        }
        else if (i != 8) {
            if (solve(m, i+1, 0)) return true;
        }
    }

    for (cand = 1; cand < 10; cand++){
        if (check(m, cand, i , j)){
            m[i][j] = cand;
            if (j != 8){
                if (solve(m, i, j+1)) return true; 
            }
            else if (i != 8){
                if (solve(m, i+1, 0)) return true;
            }
            
            if (!(i == 8 && j == 8)) m[i][j] = 0;
        }
    }
    if (i == 8 && j == 8){
        stampa_matrice(m, 9, 9);
        return true;
    }

    return false;
}
int main(int argc, char* argv[]){
    int **m;
    alloc_mat(&m);
    read_file(m);
    solve(m, 0, 0);
    printf("\n");
    return 0;
}