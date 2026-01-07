#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
#define FILE_MAP "mappa.txt"
#define FILE_PROP "proposta.txt"

typedef struct {
    int r; int c;
}cell;

int **alloc_map (int nr, int nc){
    int **m = malloc(nr*sizeof(int*));
    for (int i = 0; i < nr; i ++){
        m[i] = calloc(nc, sizeof(int));
    }
    return m;
}


int **read_map(char *filename, int *nr, int *nc){
    FILE *fp = fopen(filename, "r");
    fscanf(fp, "%d %d", nr, nc);
    int **map = alloc_map(nr, nc);
    int i, j;
    while (fscanf(fp, "%d %d", &i, &j) == 2){
        map[i][j] = -1;
    }
    return map;
}

bool bfs (int **sol, int **map, int sr, int sc, int NR, int NC, int k, int id, int **check, int flag){
    bool valid = true;
    int **dist = malloc(NR*sizeof(int*));
    for (int i = 0; i < NR; i++){
        dist[i] = malloc(NC*sizeof(int));
        for (int j = 0; j < NC; j++){
            dist[i][j] = -1;
        }
    }
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
    int head = 0, tail = 0;
    cell *queue = malloc(NR*NC*sizeof(cell));
    cell start = {sr, sc};
    queue[tail++] = start;
    dist[sr][sc] = 0;

    if (map[sr][sc] == -1) valid = false;
    int i;
    while (head < tail && valid){
        cell curr = queue[head++];
        if (map[curr.r][curr.c] == -1 || sol[curr.r][curr.c] != id) {
            valid = false; break;
        }

        if (flag == 1) check[curr.r][curr.c] = id;

        for (i = 0; i < 4; i++){
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            if (0 <= nr && nr < NR && 0 <= nc && nc < NC){
                if (dist[nr][nc] == -1){
                    dist[nr][nc] = dist[curr.r][curr.c] + 1;
                    if (dist[nr][nc] <= k && map[nr][nc] != -1){
                        cell next = {nr, nc};
                        queue[tail++] = next;
                    }
                }
            }
        }
    }
    for (i = 0; i < NR; i++){
        free(dist[i]);
    }
    free(dist);
    free(queue);
    return valid;
}


bool bfs_colora(int **partial_sol, int **map,
                int sr, int sc,
                int NR, int NC,
                int id, int k)
{
    int dist[NR][NC];
    for (int i = 0; i < NR; i++)
        for (int j = 0; j < NC; j++)
            dist[i][j] = -1;

    cell queue[NR * NC];
    int head = 0, tail = 0;

    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    /* controllo cella iniziale */
    if (map[sr][sc] == -1) return false;
    if (partial_sol[sr][sc] != 0) return false;

    queue[tail++] = (cell){sr, sc};
    dist[sr][sc] = 0;
    partial_sol[sr][sc] = id;

    while (head < tail) {
        cell curr = queue[head++];

        for (int i = 0; i < 4; i++) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            if (0 <= nr && nr < NR && 0 <= nc && nc < NC) {
                if (dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[curr.r][curr.c] + 1;

                    if (dist[nr][nc] <= k && map[nr][nc] != -1) {
                        if (partial_sol[nr][nc] != 0)
                            return false;   // overlap

                        partial_sol[nr][nc] = id;
                        queue[tail++] = (cell){nr, nc};
                    }
                }
            }
        }
    }
    return true;
}

bool create_sol(int **partial_sol, int Z, int **map, int NR, int NC, int l, int k, int **final_sol){
    if (l == Z){
        for (int i = 0; i < NR; i++){
            for (int j = 0; j < NC; j++){
                final_sol[i][j] = partial_sol[i][j];
            }
        }
        return true;
    }
    for (int i = 0; i < NR; i++) {
        for (int j = 0; j < NC; j++) {
            if (map[i][j] == -1) continue;
            if (partial_sol[i][j] != 0) continue;
            int **copy = alloc_map(NR, NC);
            for (int r = 0; r < NR; r++)
                for (int c = 0; c < NC; c++)
                    copy[r][c] = partial_sol[r][c];

            if (bfs_colora(copy, map, i, j, NR, NC, l+1, k)) {
                if (create_sol(copy, Z, map, NR, NC, l+1, k, final_sol)) {
                    /* copia soluzione valida indietro */
                    for (int r = 0; r < NR; r++)
                        for (int c = 0; c < NC; c++)
                            partial_sol[r][c] = copy[r][c];

                    for (int r = 0; r < NR; r++) free(copy[r]);
                    free(copy);
                    return true;
                }
            }
            for (int r = 0; r < NR; r++) free(copy[r]);
            free(copy);
        }
    }
    return false;
}

int **optimal_sol(int Z, int NR, int NC, int **map) {
    int **bestsol = alloc_map(NR, NC);
    int best_cov = -1;

    int **temp = alloc_map(NR, NC);
    int **curr = alloc_map(NR, NC);

    for (int k = 0; k <= NR * NC; k++) {

        // azzero temp
        for (int i = 0; i < NR; i++)
            for (int j = 0; j < NC; j++)
                temp[i][j] = 0;

        if (create_sol(temp, Z, map, NR, NC, 0, k, curr)) {
            int cov = count_coverage(curr, NR, NC);
            if (cov > best_cov) {
                best_cov = cov;
                for (int i = 0; i < NR; i++)
                    for (int j = 0; j < NC; j++)
                        bestsol[i][j] = curr[i][j];
            }
        }
    }

    // libera temp e curr
    for (int i = 0; i < NR; i++) {
        free(temp[i]);
        free(curr[i]);
    }
    free(temp);
    free(curr);

    return bestsol;
}

bool read_proposal(char *filename, int *nr, int *nc, int **map){
    FILE *fp = fopen(filename, "r");
    int i, j;
    int k, Z;
    fscanf(fp, "%d %d", &k, &Z);
    int **sources = malloc(Z*sizeof(int*));
    for (int i = 0; i < Z; i++){
        sources[i] = malloc(2*sizeof(int));
        fscanf(fp, "%d %d", &sources[i][0], &sources[i][1]);
    }
    int **sol = alloc_map(*nr, *nc);
    for(i = 0; i < *nr; i++){
        for (j = 0; j < *nc; j++){
            fscanf(fp, "%d",  &sol[i][j]);
        }
    }
    int **check = alloc_map(*nr, *nc);
    
    for (i = 0; i < Z; i++){
        if (!(bfs(sol, map, sources[i][0], sources[i][1], *nr, *nc, k, i+1, check, 1))) return false;
    }
    for (i = 0; i < *nr; i ++){
        for (j = 0; j < *nc; j++){
            if (check[i][j] != sol[i][j]){
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[]){
    int nr, nc;
    
    int **map = readmap(FILE_MAP, &nr, &nc);
    printf("\n");
    return 0;
}