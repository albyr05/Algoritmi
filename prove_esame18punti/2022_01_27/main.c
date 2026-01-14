#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
typedef struct {
    int sr;
    int er;
    int sc;
    int ec;
} region;


int **alloc_mat(int NR, int NC){
    int **m;
    return m;
}

void free_mat (int **m, int NR){

}

void read_file (int *NR, int *NC, int ***board, char *filename){
    FILE *fp = fopen(filename, "r");
    fscanf(fp, "%d %d", NR, NC);
    int i, j;
    int **m = alloc_board(NR, NC);
    for (i = 0; i < NR; i++){
        for (j = 0; j < NC; j++){
            fscanf(fp, "%d", m[i][j]);
        }
    }
    board = &m;
    return;
}

// il formato della proposta restituisce un vettore di region;
// la riga di inizio della regione
// la riga di fine della regione
// la colonna di riga della regione
// la colonna di fine della regione

bool check_proposal (region *proposal, int N_sub_region, int **board, int NR, int NC){
    bool are_square = true;
    int i, j, k, l;
    for (i = 0; i < N_sub_region && are_square; i++){
        if (!(proposal[i].er - proposal[i].sr == proposal[i].sc - proposal[i].ec)) are_square = false;
    }
    if (!are_square) return false;

    bool is_all_white = true;

    for (i = 0; i < N_sub_region; i++){
        for (j = proposal[i].sr; j < proposal[j].er; j++){
            for (k = proposal[i].sc; k < proposal[i].ec; k++){
                if (board[j][k] == 1) return false;
            }
        }
    }
    return true;
}


bool can_color (int r, int c, int size, int NR, int NC, int **board){
    if (r + size > NR || c + size > NC) return false;
    int i, j;
    for (i = r; i < r+size; i++){
        for (j = c; j < c+size; j++){
            if (board[i][j] != 0) return false;
        }
    }
    return true;
}


void color (int r, int c, int size, int **board, int id){
    int i, j;
    for (i = r; i < r+size; i++){
        for (j = c; j < c+size; j++){
            board[i][j] = id;
        }
    }

}
void best_sol(int **board, int NR, int NC, int id, int *best_sub_region){
    bool is_finished = true;
    int i, j;
    int r, c;
    if (id > *best_sub_region) return;

    for (i = 0; i < NR && is_finished; i++){
        for (j = 0; j < NC && is_finished; j++){
            if (board[i][j] == 0) {
                r = i;
                c = j;
                is_finished = false;
            }

        }
    }
    if (is_finished){
        if(id < *best_sub_region) *best_sub_region = id;
        return;
    }

    int max_dim = min(NR-r, NC-c);
    for (int size = max_dim; size > 0; size--){
        if (can_color(r, c, size, NR, NC, board)){ // checking if the square of dim size can be colored 

            color_square(r, c, size, board, id);                // coloring with the current id    

            best_sol(board, NR, NC, id+1, best_sub_region);         // recursion with, starting a new region 

            color_square(r, c, size, board, 0);         // backtraking --> putting the whole square back to zero
        }
    }
    
}


int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}