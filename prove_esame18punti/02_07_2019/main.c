#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;

int **read_matrix(char *filename);

bool check_sol(int *sol, int n, int dist_max, int **dist, int n_city){
    int i, j;
    bool flag;
    for (i = 0; i < n_city; i++){
        flag = false;
        for (j = 0; j < n; j++){
            if (dist[i][sol[j]] < dist_max) flag = true;
        }
        if (!flag) return false;
    }
    return true;
}

bool function1(int pos, int **dist, int *sol, int n_city, int dist_max, int start, int target){
    if (pos == target){
        if(check_sol(sol, pos, dist_max, dist, n_city)) return true;
    }
    int i, j;
    for (i = start; i < n_city; i++){
        sol[pos] = i;
        if(function1(pos + 1, dist, sol, n_city, dist_max, i+1, target)) return true;
    }
    return false;
}

int func1_w(int **dist, int n_city, int dist_max, int op){
    int *final_sol = malloc(n_city*sizeof(int));
    for (int k = 1; k <= n_city; k++){
        if (function1(0, dist, final_sol, n_city, dist_max, 0, k)){
            if (op == 0){
                for (int j = 0; j < k; j++){
                    printf("%d", final_sol[j]);
                }
            }
            free(final_sol);
            return k;
        }
    }
    free(final_sol);
    return -1;
}

void func2(int pos, int *sol, int **dist, int n_city, int *pop, int N_staz, int *min, int *stazcomuni, int start){
    if (pos == N_staz) {
        int val = 0;
        for (int i = 0; i < n_city; i++){
            int min_temp = sol[0];
            for (int k = 1; k < pos; k++){
                if (dist[i][sol[k]] < dist[i][min_temp]) min_temp = sol[k];
            }
            int cnt = 0;
            for (int k = 0; k < pos; k++){
                if (sol[k] == min_temp) cnt ++;
            }
            val += (pop[i]*dist[i][min_temp] / cnt);
        }
        if (val < *min ) *min = val;
        return;
    }

    int i ;
    for (i = start; i < n_city; i++){
        if (stazcomuni[i] > 0){
            stazcomuni[i] --;
            sol[pos] = i;
            func2(pos+1, sol, dist, n_city, pop, N_staz, min, stazcomuni, i);
            stazcomuni[i]++;
        }
    }
    return;
}
// formato proposta:
// N_stazminima Dist_max
// seguono N_staz righe riportanti l'indice della città
bool check_prop(char *fileprop, int **dist, int n_city){
    int i, j, k;
    FILE *fp = fopen(fileprop, "r");
    int min, dist_max;
    fscanf(fp, "%d %d", &min, &dist_max);
    int *city = malloc(min*sizeof(int));
    for (i= 0; i < min; i++){
        fscanf(fp, "%d", &city[i]);
    }
    if (!check_sol(city, min, dist_max, **dist, n_city)) return false;
    if(func1_w(dist, n_city, dist_max, 1) != min) return false;

    return true;

}

int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}