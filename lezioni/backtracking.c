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

int main(){
    int n = 4, k = 2;
    int *val = malloc(n*sizeof(int));
    for (int i = 0; i < n; i++){
        val[i] = i;
    }
    int *mark = calloc(n, sizeof(int));
    int *sol = malloc(k*sizeof(int));
    int cnt = 0, pos = 0;
    cnt = disp_semplici(pos, val, mark, sol, cnt, n, k);
    printf("\nCi sono %d disposizioni semplici di %d elementi in classe %d", cnt, n, k);


    return 0;
}