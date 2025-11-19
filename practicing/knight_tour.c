#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;

int valid_moves[8][2] = {
    { 2,  1}, { 1,  2},
    {-1,  2}, {-2,  1},
    {-2, -1}, {-1, -2},
    { 1, -2}, { 2, -1}
};

void alloc_mat(int ***board, int N){
    int **m = malloc(N*sizeof(int*));
    for (int i = 0; i < N; i++){
        m[i] = calloc(N, sizeof(int));
    }
    *board = m;
}
void free_m (int **board, int N){
    for (int i = 0; i < N; i++){
        free(board[i]);
    }
    free(board);
}



void printboard(int ** board, int N){
    for (int i = 0; i< N; i++){
        for (int j = 0; j < N; j++){
            printf("%d ", board[i][j]);
        }
        printf ("\n");
    }
    return;
}

bool solution (int **board, int i, int j, int cnt, int N){
    int k;
    if (cnt == N*N){
        printboard (board, N);
        return true;
    }   
    for (k = 0; k<8; k++){
        int n_i = i + valid_moves[k][0];
        int n_j = j + valid_moves[k][1];
        if ((0 <= n_i && n_i< N) && (0 <= n_j && n_j < N) && (board[n_i][n_j] == 0)){
            cnt ++;
            board[n_i][n_j] = cnt;
            if (solution(board, n_i, n_j, cnt, N)) return true; 
            board[n_i][n_j] = 0;
            cnt--;
        }
    }
    return false;
}
void wrapper(int **board, int N){
    int cnt = 1;
    board[0][0] = 1;
    solution(board, 0, 0, cnt, N);
}

int main(int argc, char* argv[]){
    int N = 8;
    int **board;
    alloc_mat(&board, N);
    wrapper(board, N);
    printf("\n");
    return 0;
}