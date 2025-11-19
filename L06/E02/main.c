#include<stdio.h>
#include<stdlib.h>
// z -> z,r 
// r -> s, t;
// t -> z,r
// s -> s, t; 

typedef enum{false, true} bool;

int fz(int ***** memo, int Z, int R, int T, int S);
int fr(int ***** memo,int Z, int R, int T, int S);
int ft(int ***** memo, int Z, int R, int T, int S);
int fs(int ***** memo, int Z, int R, int T, int S);

// allocating the 5D matrix 
void alloc_memo(int ******m, int Z, int R, int T, int S){
    int *****memo = malloc(4*sizeof(int****));

    int i, z, r, s, t;
    for (i = 0; i < 4; i++){
        memo[i] = malloc((Z+1)*sizeof(int***));
        for (z = 0; z <= Z; z++){
            memo[i][z] = malloc((R+1)*sizeof(int**));
            for (r=0; r<=R; r++){
                memo[i][z][r] = malloc((T+1)*sizeof(int*));
                for (t = 0; t <= T; t++){
                    memo[i][z][r][t] = malloc((S+1)*sizeof(int));
                    for (s = 0; s <= S; s++){
                        memo[i][z][r][t][s] = -1;
                    }
                }
            }
        }
    }
    *m = memo;
    return;
}
// freeing the 5D matrix
void free_memo (int *****memo, int Z, int R, int T, int S){
    int t, r, z, i;
    for (i = 0; i < 4; i++){
        for (z = 0; z <= Z; z++){
            for (r = 0; r <= R; r++){
                for (t = 0; t <= T; t++){
                    free(memo[i][z][r][t]);
                }
                free(memo[i][z][r]);
            }
            free(memo[i][z]);
        }
        free(memo[i]);
    }
    free(memo);
    return;
}

int max (int v1, int v2){
    if (v1 > v2) return v1;
    return v2;
}

// all the four function are the same
int fz(int ***** memo, int Z, int R, int T, int S){
    if (Z == 0) return 0;       // if no more of the stone then i can't go on
    if (memo[0][Z][R][T][S] != -1) return memo[0][Z][R][T][S];      // if the sol has already been calcualted I can return
    Z--;        // consuming a stone kind
    int best = 1 + max(fz(memo, Z, R, T, S), fr(memo, Z, R, T, S));         // calculating the best of the the 2 possivle brenches
    Z ++;       // backtrack
    return memo[0][Z][R][T][S] = best;  // returning the best and also saving it for the future
    
}
int fr(int ***** memo, int Z, int R, int T, int S){
    if (R == 0) return 0;
    if (memo[1][Z][R][T][S] != -1) return memo[1][Z][R][T][S];
    R--;
    int best = 1 + max(fs(memo, Z, R, T, S), ft(memo, Z, R, T, S));
    R ++;
    return memo[1][Z][R][T][S] = best;
    
}
int ft(int ***** memo, int Z, int R, int T, int S){
    if (T == 0) return 0;
    if (memo[2][Z][R][T][S] != -1) return memo[2][Z][R][T][S];
    T--;
    int best = 1 + max(fz(memo, Z, R, T, S), fr(memo, Z, R, T, S));
    T ++;
    return memo[2][Z][R][T][S] = best;
    
}
int fs(int ***** memo, int Z, int R, int T, int S){
    if (S == 0) return 0;
    if (memo[3][Z][R][T][S] != -1) return memo[3][Z][R][T][S];
    S--;
    int best = 1 + max(fs(memo, Z, R, T, S), ft(memo, Z, R, T, S));
    S ++;
    return memo[3][Z][R][T][S] = best;
    
}

int sol_wrapper(int *****memo, int Z, int R, int T, int S){
    int max_z = fz(memo, Z, R, T, S);       // staring with all the stone kind
    int max_r = fr(memo, Z, R, T, S);
    int max_t = ft(memo, Z, R, T, S);
    int max_s = fs(memo, Z, R, T, S);
    int sol[4] = {max_z, max_r, max_t, max_s};
    int bs = 0;
    for (int i = 0; i < 4; i++){    
        if (sol[i] > bs){
            bs = sol[i];        // looking for the best
        }           
    }
    return bs;
}

void readfile (){
    FILE *fp;
    if ((fp = fopen("test.txt", "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "test.txt");
        exit(-1);
    }
    int nr, Z, R, T, S;
    fscanf(fp, "%d", &nr);
    for (int i = 0; i < nr; i++){
        fscanf(fp, "%d %d %d %d", &Z, &R, &T, &S);
        int *****memo;
        alloc_memo(&memo, Z, R, T, S);
        int bs = sol_wrapper(memo, Z, R, T, S);
        printf("The best length for row %d is %d\n", i+1, bs);
        free_memo(memo, Z, R, T, S);
    }

    fclose(fp);
}
int main(int argc, char* argv[]){
    readfile();
    printf("\n");
    return 0;
}