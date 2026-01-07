#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
typedef struct {
    int r, c, d;
}cell;

int **alloc_map (int nr, int nc){
    int **m = malloc(nr*sizeof(int*));
    for (int i = 0; i < nr; i ++){
        m[i] = calloc(nc, sizeof(int));
    }
    return m;
}


bool bfs(int **map, int sr, int sc, int R, int C, int id, int K){
    if (map[sr][sc] > 0) return false;

    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
    int head = 0, tail = 0;
    cell *queue = malloc(R*C*sizeof(cell));
    queue[tail++] = (cell) {sr, sc, 0};
    map[sr][sc] = id;
    
    while (head < tail){
        cell current = queue[head++];

        for (int i = 0 ; i < 4; i++){
            int nr = current.r + dr[i];
            int nc = current.c + dc[i];
            if (0 <= nr && nr < R && 0 <= nc && nc < C){
                if (map[nr][nc] != id){

                    if (map[nr][nc] == -1) continue;
    
                    if (map[nr][nc] > 0){
                        free(queue);
                        return false;
                    }
    
                    else if (map[nr][nc] == -2) map[nr][nc] = id;
    
                    else{
                        cell next = {nr, nc, current.d+1};
                        map[nr][nc] = id;
                        if (next.d < K){
                            queue[tail++] = next;
                        }
                    }
                }
            }
        }
    }
    free(queue);
    return true;
}

void copy(int **dest, int **m, int R, int C){
    int i, j;
    for (i = 0; i < R; i++){
        for (j = 0; j < C; j++){
            dest[i][j] = m[i][j];
        }
    }
}

bool solve(int **curr_map, int R, int C, int K, int id, int Z, int **finalsol){
    if (id == Z){
        copy(finalsol, curr_map, R, C);
        return true;
    }

    int ** backup = alloc_map(R, C);
    int i, j;
    for (i = 0; i < R; i ++){
        for (j = 0; j < C; j++){
            if(curr_map[i][j] != 0) continue;
            copy(backup, curr_map, R, C);
            if (bfs(backup, i, j, R, C, id+1, K)){
                if (solve(backup, R, C, K, id+1, Z, finalsol)){
                    for (int m = 0; m < R; m++){
                        free(backup[m]);
                    }
                    free(backup);
                    return true;
                }
            }
        }
    }
    for (int m = 0; m < R; m++){
        free(backup[m]);
    }
    free(backup);
    return false;
}


int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}