#include <stdio.h>
#include <stdlib.h>


int disp_semplici(int pos, int *val, int *mark, int *sol, int cnt, int n, int k){
    if (pos >= k){
        for (int i = 0; i < k; i ++){
            printf ("%d", sol[i]);
        }
        printf("\n");
        return cnt + 1;
    }
    for (int i = 0; i < n; i++){
        if (mark[i] == 0){
            sol[pos] = val[i];
            mark[i] = 1;
            cnt = disp_semplici(pos+1, val, mark, sol, cnt, n, k);
            mark[i] =0;
        }
    }
    return cnt;
}

int disp_rip(int pos, int *val, int *sol, int cnt, int n, int k){
    int i;
    if (pos >= k){
        for (i = 0; i < k; i ++){
            printf("%d", sol[i]);
        }
        printf("\n");
        return cnt +1;
    }
    for (i = 0; i<n; i++){
        sol[pos] = val[i];
        cnt = disp_rip(pos+1, val, sol, cnt, n, k);
    }
    return cnt;
}

int perm(int pos, int *val, int *sol, int* mark,  int cnt, int n){
    int i;
    if (pos >= n){
        for (i = 0; i<n; i++){
            printf("%d", sol[i]);
        }
        printf("\n");
        return cnt +1;
    }
    for (i = 0; i < n; i++){
        if (mark[i] == 0){
            mark[i] = 1;
            sol[pos] = val[i];
            cnt = perm(pos+1, val, sol, mark, cnt, n);
            mark[i] = 0;
        }
    }
    return cnt;
}

int main(){
    int n = 4, k = 2;
    int *val = malloc(n*sizeof(int));
    for (int i = 0; i < n; i++){
        val[i] = i;
    }
    int *mark = calloc(n, sizeof(int));
    int *sol = malloc(k*sizeof(int));
    int cnt = 0, pos = 0;

    // disp semplici
    cnt = disp_semplici(pos, val, mark, sol, cnt, n, k);
    printf("\nCi sono %d disposizioni semplici di %d elementi in classe %d\n", cnt, n, k);
    
    // disp rip
    cnt = disp_rip(pos, val, sol, 0, n, k);
    printf("\nCi sono %d disposizioni con ripetizioni di %d elementi in classe %d\n", cnt, n, k);

    // perm semplici
    free(sol);
    sol = malloc(n*sizeof(int));
    cnt = perm(pos, val, sol, mark, 0, n);
    printf("ci sono %d permutazioni di %d elementi", cnt, n);
    return 0;
}