#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
int dir[4][2] = {{0,1}, {1,0}, {-1, 0}, {0, -1}};

void find_max_path(int **maze, int **path, int i, int j, int nr, int nc, int *max_val, int curr_val, int **visited){
    int k;
    if (i == nr-1 && j == nc-1){
        if (curr_val > *max_val) {
            *max_val = curr_val;
            for (int i = 0; i < nr; i++){
                for (int j = 0; j < nc; j++){
                    path[i][j] = visited[i][j];
                }
            }
        }
        return;
    }

    for (k = 0; k < 4; k++){
        int n_i = i + dir[k][0];
        int n_j = j + dir[k][1];
        if ((0 <= n_i && n_i < nr) && (0 <= n_j && n_j < nc)){
            if (maze[n_i][n_j] > maze[i][j] && !visited[n_i][n_j]){
                int tmp = maze[n_i][n_j];
                curr_val += tmp;
                visited[n_i][n_j] = 1;
                find_max_path(maze, path, n_i, n_j, nr, nc, max_val, curr_val, visited);
                curr_val -= tmp;
                visited[n_i][n_j] = 0;

            }
        }
    }
    return;
}

int wrapper (int **maze, int nr, int nc){
    int **visited = malloc(nr * sizeof(int*));
    for (int i = 0; i < nr; i++) {
        visited[i] = calloc(nc, sizeof(int)); // inizializza a 0
    }
    int **path = malloc(nr * sizeof(int*));
    for (int i = 0; i < nr; i++) {
        path[i] = calloc(nc, sizeof(int)); // inizializza a 0
    }
    path[0][0] = 1 ;
    visited[0][0] = 1;
    int max_value = 0;
    find_max_path(maze, path, 0, 0, nr, nc, &max_value, maze[0][0], visited);
    for (int i = 0;i < nr; i ++){
        for (int j= 0; j < nc; j++){
            if (path[i][j] == 1){
                printf ("(%d, %d) ->", i, j);
            }
        }
    }
    printf("\n");


    return max_value;
}

void read_file(int ***maze, int *nr, int *nc){
    FILE *fp;
    if ((fp = fopen("maze.txt", "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "maze.txt");
        exit(-1);
    }
    fscanf(fp, "%d %d", nr, nc);
    int **board = malloc((*nr)*sizeof(int*));
    for (int i = 0; i < *nr; i++){
        board[i] = malloc((*nc)*sizeof(int));
    }
    for (int i = 0; i< *nr; i++){
        for (int j = 0; j< *nc; j++){
            fscanf(fp, "%d", &board[i][j]);
        }
    }
    *maze = board;
    
    fclose(fp);
}

int main(int argc, char* argv[]){
    int **maze;
    int nr, nc;
    read_file(&maze, &nr, &nc);
    int max_value = wrapper(maze, nr, nc);
    printf("Percorso con lunghezza massima %d", max_value);
    printf("\n");
    return 0;
}