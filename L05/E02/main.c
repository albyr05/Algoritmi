#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
typedef struct{
    char colour[2];
    int value[2];
    bool rotate;
    bool present;
} tile;

void free_board(tile **m, int nr){
    for (int i = 0; i < nr; i++){
        free(m[i]);
    }
    free(m);
    return;
}

void rotate_tile(tile *a){
    char tmpc = a->colour[0]; char tmpv = a->value[0];
    a->colour[0] = a->colour[1]; a->colour[1] = tmpc;
    a->value[0] = a->value[1]; a->value[1] = tmpv;
}

int calculate_val_board(tile **board, int nr, int nc){
    int i, j;
    char col;
    int val_tot = 0; int val_tmp;
    for (i = 0; i < nr; i++){
        col = board[i][0].colour[0];
        val_tmp = board[i][0].value[0];
        bool flag = true;
        for (j = 1; j < nc && flag; j++){
            if (board[i][j].colour[0] == col){
                val_tmp += board[i][j].value[0];
            }
            else flag = false;
        }
        if (flag) val_tot += val_tmp;
    }
    for (j = 0; j < nc; j++){
        col = board[0][j].colour[1];
        val_tmp = board[0][j].value[1];
        bool flag = true;
        for (i = 1; i < nr && flag; i++){
            if (board[i][j].colour[1] == col){
                val_tmp += board[i][j].value[1];
            }
            else flag = false;
        }
        if (flag) val_tot += val_tmp;
    }
    return val_tot;
}

void printboard (tile**board, int nr, int nc){
    int i, j;
    for (i = 0; i < nr; i++){
        for (j = 0; j < nc; j++){
            if (board[i][j].present == true){
                printf("%c %d %c %d      ", board[i][j].colour[0], board[i][j].value[0], board[i][j].colour[1], board[i][j].value[1]);
            }
            else printf("EMPTY   ");
        }
        printf("\n");
    }
    return;
}

void best_board(tile **board, tile **best_sol, tile *tiles, int nr, int nc, int i, int j, int *max_val, int N){
    
    // end of board --> can evaluate its value
    if (i == nr) {
        int current_val = calculate_val_board(board, nr, nc);
        if (current_val > *max_val) {
            *max_val = current_val;
            for (int k = 0; k < nr; k++){
                for (int l = 0; l< nc; l++){
                    best_sol[k][l] = board[k][l];
                }
            }
        }
        return;
    }

    // calculating the next cell to check
    int next_i = i + (j + 1) / nc;
    int next_j = (j + 1) % nc;

    // case i already have the tile fixed in the board
    if (board[i][j].present == true) {

        // try positioning next tales without rotating the current one
        best_board(board, best_sol, tiles, nr, nc, next_i, next_j, max_val, N);

        // rotating the current
        rotate_tile(&board[i][j]);

        // try positioning next tales after rotation of the current
        best_board(board, best_sol, tiles, nr, nc, next_i, next_j, max_val, N);

        // BACKTRACK rotatin back the current one
        rotate_tile(&board[i][j]);
        return;         //return beacuse i already have explored all the brenches 
    }   

    // trying all the unused tales 
    for (int k = 0; k < N; k++) {
        if (!tiles[k].present) {

            tiles[k].present = true;    // marking the tale
            board[i][j] = tiles[k];     // placing in the board 
            
            // try moving on without rotating 
            best_board(board, best_sol, tiles, nr, nc, next_i, next_j, max_val, N);

            // try with rotation
            rotate_tile(&board[i][j]);
            best_board(board, best_sol, tiles, nr, nc, next_i, next_j, max_val, N);

            // BACKTRACK rotating back the tile
            rotate_tile(&board[i][j]);

            // BACKTRACK removing the insert tile 
            tiles[k].present = false;
            board[i][j].present = false;
        }
    }
    return;
}

int best_board_wrapper(tile ** board, tile *tiles, int *nr, int *nc, int N){
    int max_val = 0;
    tile **best_sol = malloc((*nr)*sizeof(tile*));
    for (int i = 0; i < *nr; i ++){
        best_sol[i] = malloc((*nc)*sizeof(tile));
    }
    best_board(board, best_sol, tiles, *nr, *nc, 0, 0, &max_val, N);
    printboard(best_sol, *nr, *nc);
    free_board(best_sol, *nr);
    return max_val;
}

void read_board(tile *tiles, tile ***matr, int *nr, int *nc){
    FILE *fp;
    int i, j;
    if ((fp = fopen("board.txt", "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "board.txt");
        exit(-1);
    }
    fscanf(fp, "%d %d", nr, nc);
    int card, rotation;
    tile **board = malloc((*nr)*sizeof(tile*));
    for (i = 0; i < *nr; i++){
        board[i] = malloc((*nc)*sizeof(tile));
    }
    for (i = 0; i < *nr; i ++){
        for (j = 0; j < *nc; j++){
            fscanf(fp, "%d/%d", &card, &rotation);
            if (rotation != -1 && card != -1){
                tiles[card].present = true;
                if (rotation == 0){
                    tiles[card].rotate = false;
                }
                else {
                    tiles[card].rotate = true;
                    rotate_tile(&tiles[card]);
                }
                board[i][j] = tiles[card];
            }
            else{
                board[i][j].present = false;        // così so che la tessera nn è presente
            }
        }
    }

    *matr = board;
    fclose(fp);
}

int read_tiles(tile ** cards){
    FILE *fp;
    int i;
    if ((fp = fopen("tiles.txt", "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "tiles.txt");
        exit(-1);
    }
    int N_tiles;
    fscanf(fp, "%d", &N_tiles);
    tile *tiles = malloc(N_tiles*sizeof(tile));
    for (i = 0; i < N_tiles; i++){
        fscanf(fp, " %c %d %c %d", &tiles[i].colour[0], &tiles[i].value[0], &tiles[i].colour[1], &tiles[i].value[1]);
        tiles[i].present = false; 
    }
    fclose(fp);
    *cards = tiles;
    return N_tiles;
}


int main(int argc, char* argv[]){
    tile * tiles;
    int N = read_tiles(&tiles);
    tile **board;
    int nr, nc;
    read_board(tiles, &board, &nr, &nc);
    int max_val = best_board_wrapper(board, tiles, &nr, &nc, N);
    printf("IL VALORE MASSIMO DELLA SCACCHIERA È %d", max_val);
    printf("\n");
    free_board(board, nr);
    free(tiles);
    return 0;
}