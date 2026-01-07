#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;

typedef struct {
    int n;
    int s; 
    int o;
    int e;
}cell;

typedef struct {
    int dr;
    int dc;
}move;


cell **alloc_grid(int N);

bool isempty(cell x){
    if (x.n == 0 && x.o == 0 && x.s == 0 && x.e == 0) return true;
    return false;
}

cell set_empty (){
    
}
bool is_source(cell x){

}

cell **readfile(char *filename){
    FILE *fp = fopen(filename, "r");
    int N, T;
    fscanf(fp, "%d %d", &N, &T);
    int i;
    cell **grid = alloc_grid(N);
    for (i = 0; i < N*N; i++){
        int row = i/N;
        int col = i%N;
        fscanf(fp, "%d %d %d %d", &grid[row][col].n, &grid[row][col].s, &grid[row][col].o, &grid[row][col].e);
    }
    fclose(fp);
    return grid;
}

bool in_boundaries(int new_r, int new_c, int N){
    if (0 <= new_r && new_r < N && 0 <= new_c && new_c < N) return true;
    return false;
}

bool move_board (cell **grid, move m, int N){
    int i, j;
    bool one_movement = false;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if (!isempty(grid[i][j])){
                int new_r = i + m.dr;
                int new_c = j + m.dc;
                if (in_boundaries(new_r, new_c, N)){
                    if (isempty(grid[new_r][new_c])){
                        grid[new_r][new_c] = grid[i][j];
                        grid[i][j] = set_empty();
                        one_movement = true;
                    }
                }
            }
        }
    }
    return one_movement;
}

bool compatible(cell a, cell b){
    if (isempty(a) || isempty(b)) return false;
    if (a.e == 1 && b.o == 1 || a.o == 1 && b.e == 1) return true;
    if (a.s == 1 && b.n == 1 || a.n == 1 && b.s == 1) return true;
    return false;
}

bool dfs(cell **grid, int r, int c, bool **visited, int N, int sr, int sc){
    visited[r][c] = true;

    if (is_source(grid[r][c]) &&  r != sr && c != sc) return true;

    int dr[4] = {0, 0, 1, -1};
    int dc[4] = {1, -1, 0, 0};

    int i;
    for (i = 0; i < 4; i++){
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (in_boundaries(nr, nc, N) && !visited[nr][nc]){
            if (compatible(grid[r][c], grid[nr][nc])){
                if (dfs(grid, nr, nc, visited, N, sr, sc)) return true;
            }
        }
    }
    return false;
}

bool check_sol(cell **grid, int N){
    int i, j;
    int sr = -1, sc = -1;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            if (is_source(grid[i][j])) {
                sr = i;
                sc = j;
                break;
            }
            if (sr != -1) break;
        }
    }
    bool **visited = malloc(N*sizeof(bool*));
    for (i = 0; i < N; i++){
        visited[i] = malloc(N*sizeof(bool));
        for (j = 0; j < N; j++) visited[i][j] = false;
    }

    if (dfs(grid, sr, sc, visited, N, sr, sc)) return true;

    return false;
}

bool read_proposal(char *filename, cell **grid, int N){
    FILE *fp = fopen(filename, "r");
    int N_moves;
    fscanf(fp, "%d", &N_moves);
    move *moves = malloc(N_moves*sizeof(move));
    int i ;
    for (i = 0; i < N_moves; i++){
        fscanf(fp, "%d", &moves[i].dr, &moves[i].dc);
    }


    bool valid_moves = true;

    for (i = 0; i < N_moves; i++){
        if (!move_board(grid, moves[i], N))  valid_moves = false;
    }
    if (!valid_moves) return false;


    check_sol(grid, N);
}

bool generate_sol(cell **grid, int pos, move *moves, int *dr, int *dc, int k, int N){
    if (pos == k){
        if (check_sol(grid, N)) return true;
        return false;
    }

    int i;
    cell **copy;
    for (i = 0; i < 4; i++){
        copy_status(copy, grid, N);
        move m = {dr[i], dc[i]};
        if (!move_board(copy, m, N)) continue;
        moves[i] = m;
        if (generate_sol(copy, pos+1, moves, dr, dc, k, N)){
            free(copy);
            return true;
        }
    }
    free(copy);
    return false;
}

void gene_wrap(cell **grid, int M, int N){
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    move *moves = malloc(M*sizeof(move));
    int k;
    for (k = 0; k < M; k++){
        generate_sol(grid, 0, moves, dr, dc, k, N);
    }
}


int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}