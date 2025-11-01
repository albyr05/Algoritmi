#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

bool check_vertexcover(int *sol, int **arches, int n_arches, int pos){
    int i, k; bool check;
    for (i = 0; i < n_arches; i++){
        check = false;
        for(k = 0; k < pos && check == false; k++){
            if (arches[i][0] == sol[k] || arches[i][1] == sol[k]){          // condition to be vertex cover
                check = true;          // one of the vertex of the arch is in the powerset so i break the inner cycle and go into an other arch
            }
        }
        if (!check) return false;
    }
    return true;
}

// creating all the powerset 
int powerset(int pos, int *val, int *sol, int n_vert, int cnt, int** arches, int n_arches, int start){
    int i; 
    if (start >= n_vert){
        if (check_vertexcover(sol, arches, n_arches, pos) > 0){         // validating if the powerset is a vertex cover
            for (i = 0; i < pos; i++){
                printf("%d ", sol[i]);
            }
            printf("\n");
        }
        return cnt +1;
    }
    for (i = start; i < n_vert; i++){
        sol[pos] = val[i];
        cnt = powerset(pos+1, val, sol, n_vert, cnt, arches, n_arches, i+1);
    }
    cnt = powerset(pos, val, sol, n_vert, cnt, arches, n_arches, n_vert);
    return cnt;
}

int main(){
    FILE *fp = fopen("es1.txt", "r");
    int N_vert, n_arches, i;
    fscanf(fp, "%d %d", &N_vert, &n_arches);

    int **arches = malloc(n_arches*sizeof(int*));
    for (i = 0; i < n_arches; i++){
        arches[i] = malloc(2*sizeof(int));          // populating the matrix that contains the arches 
        fscanf(fp, "%d %d", &arches[i][0], &arches[i][1]);
    }

    int *vertex = malloc(N_vert*sizeof(int));       // creating the vertex vector
    for (i = 0; i < N_vert; i++){
        vertex[i] = i;
    }
    int *sol = malloc(N_vert*sizeof(int));      // allocating the solution vector

    int cnt = 0;
    cnt = powerset(0, vertex, sol, N_vert, cnt, arches, n_arches, 0);
    printf("%d", cnt);
    return 0;

}