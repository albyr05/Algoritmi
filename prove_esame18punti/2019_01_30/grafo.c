#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;

typedef struct {
    int V;
    int **madj;
    char **ST;
}*Graph;

Graph graph_init(int V);


Graph read_file (char *filename1){
    FILE *fp = fopen(filename1, "r");
    char id1[20], id2[20];
    int cnt = 0;
    while (fscanf(fp, "%s %s", id1, id2) == 2);{
        cnt++;
    }
    rewind(fp);
    Graph G = graph_init(2*cnt);
    for (int i = 0; i < cnt; i++){
        fscanf(fp, "%s %s", id1, id2);
        int u = getindex(id1, G->ST);
        int v = getindex(id2, G->ST);
        G->madj[u][v] = 1; 
    }
    return G;
}

bool check (int *possible_kernel, int nv, Graph G){
    int i, j, l, m;

    bool flag1 = false;
    bool flag2 = false;
    for (i = 0; i < nv && !flag1; i++){
        for (j = 0; j < nv && !flag1; j++){
            if (G->madj[possible_kernel[i]][possible_kernel[j]] == 1 || G->madj[possible_kernel[j]][possible_kernel[i]] == 1) flag1 = true;
        }
    }
    if (flag1){
        free(possible_kernel);
        return false;
    }

    else{
        for(i = 0; i < G->V; i++){
            bool in = false;
            bool correct = false;
            for (l = 0; l < nv; l++){
                if (possible_kernel[l] == i) in = true;
            }
            if (in) continue;
            if (!in) {
                for (l = 0; l < nv; l++){
                    if (G->madj[l][i] == 1) correct = true;
                }
            }
            if (!correct){
                free(possible_kernel); 
                return false;
            }
        }
        free(possible_kernel);
        return true;
    }
}


bool check_kernel (Graph G, char *filename2){
    FILE *fp = fopen(filename2, "r");
    int nv;
    int i, j, l, m;
    fscanf(fp, "%d", &nv);
    int *possible_kernel = malloc(nv*sizeof(int));
    for (i = 0; i < nv; i++){
        fscanf(fp, "%d", &possible_kernel[i]);
    }
    return check(possible_kernel, nv, G);
}

bool generate_kernel(int *sol, int k, Graph G, int pos, int start, int *best_sol){
    if (pos >= k){
        if(check(sol, pos, G)){
            for (int i = 0 ; i < pos; i++){
                best_sol[i] = sol[i]; 
            }
            return true;
        }
        return false;
    }
    int i;
    for (i = start; i < G->V; i++){
        bool can_add = true;
        for (int j = 0; j < pos && can_add; j++){
            if (G->madj[i][sol[j]] == 1 || G->madj[sol[j]][i] == 1) can_add = false;
        }
        if(can_add){
            sol[pos] = i;
            if (generate_kernel(sol, k, G, pos+1, i+1, best_sol)) return true;
        }
    }
    return false;
}

void wrap_generate_sol(Graph G, char *filename3){
    FILE *fp = fopen(filename3, "w");
    int *best_sol = malloc(G->V*sizeof(int));
    int *sol = malloc(G->V*sizeof(int));
    for (int k = 0; k < G->V; k++){
        if (generate_kernel(sol, k, G, 0, 0, best_sol)){
            for (int i = 0; i < k; i++){
                fprintf(fp, "%d\n", best_sol[i]);
            }
            break;
        }
    }
    free(sol); free(best_sol);
}


int main(int argc, char const *argv[]){
    if (argc != 4){
        return -1;
    }
    Graph G = read_file(argv[1]);
    check_kernel(G, argv[2]);
    wrap_generate_sol(G, argv[3]);
    return 0;
}
