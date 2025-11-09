#include<stdio.h>
#include<stdlib.h>
#include <string.h>
typedef enum{false, true} bool;
typedef struct {
    char name[30];
    int val;
    bool present;
}animal;


void alloc_board(int ***board, int nr, int nc){
    int **m = malloc(nr*sizeof(int*));
    for (int i = 0; i < nr; i++){
        m[i] = malloc(nc*sizeof(int));
    }
    *board = m;
}

void freeboard (int **board, int nr){
    for (int i = 0; i < nr; i++){
        free(board[i]);
    }
    free(board);
}
int evaluate_board(int **board, animal *v, int nr, int nc){
    // evaluate row 
    int i, j;
    int val = 0, tmp;
    for (i = 0; i < nr; i++){
        bool flag = true;
        tmp = v[board[i][0]].val;
        for (j = 1; j < nc && flag; j++){
            if (v[board[i][j]].val > v[board[i][j-1]].val){
                tmp += v[board[i][j]].val;
            }
            else flag = false;
        }
        if (flag) val+= tmp;
    }
    // evaluate column
    for (j = 0; j < nc; j++){
        bool flag = true;
        for (i = 1; i < nr && true; i++){
            if (strcmp(v[board[i][j]].name, v[board[i-1][j]].name) == 0) flag = false; 
        }
        if (flag) val += (nr*5);
    }
    return val;
}
void create_board(int i, int j, int **board, int **best_board, animal *v, int nr, int nc, int *maxval, int N){
    int k, l;
    if (i == nr){
        int val = evaluate_board(board, v, nr, nc);
        if ( val > *maxval) {
            *maxval = val;
            for (k = 0; k < nr; k++){
                for (l = 0; l < nc; l++){
                    best_board[k][l] = board[k][l];
                }
            }
        }
        return;
    }
    int n_i = i + (j+1) / nc;
    int n_j = (j+1) % nc;
    for (k = 0; k < N; k++){
        if (v[k].present == false){
            board[i][j] = k;
            v[k].present = true;
            create_board(n_i, n_j, board, best_board, v, nr, nc, maxval, N);
            v[k].present = false;
        }
    }
    return;

}
void print_sol(int **b, animal*v, int nr, int nc){
    int i, j;
    for (i = 0; i < nr; i++){
        for (j = 0; j < nc; j++){
            printf ("%s   ", v[b[i][j]].name);
        }
        printf("\n");
    }
}
void sol_wrapper(int **board, animal *v, int nr, int nc, int N){
    int ** best_board;
    alloc_board(&best_board, nr, nc);
    int maxval = 0;
    create_board(0,0, board, best_board, v, nr, nc, &maxval, N);
    printf("Best wellness value: %d\n", maxval);
    print_sol(best_board, v, nr, nc);
    freeboard(best_board, nr);
}
int readfile(animal **vect){
    FILE *fp;
    if ((fp = fopen("zoo.txt", "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "zoo.txt");
        exit(-1);
    }
    int N; 
    fscanf(fp, "%d", &N);
    animal *v = malloc(N*sizeof(animal));
    for (int i= 0; i< N; i++){
        fscanf(fp, "%s %d", v[i].name, &v[i].val);
        v[i].present = false;
    }
    fclose(fp);
    *vect = v;
    return N;
}



int main(int argc, char* argv[]){
    int nr = 2, nc = 3;
    animal *v;
    int N = readfile(&v);
    int **board;
    alloc_board(&board, nr, nc);
    sol_wrapper(board, v, nr, nc, N);
    free(v);
    freeboard(board, nr);
    printf("\n");
    return 0;
}