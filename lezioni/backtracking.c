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

int comb_semplici(int pos, int *val, int *sol, int n, int k, int start, int cnt){
    int i; 
    if (pos >= k){
        for (i = 0; i <k; i ++){
            printf("%d", sol[i]);
        }
        printf("\n");
        return cnt +1;
    }   
    for (i = start; i < n; i ++){
        sol[pos] = val[i];
        cnt = comb_semplici(pos+1, val, sol, n, k, i+1, cnt); // passondogli i+1, l'elemento i-esimo appena inserito, non verrà inserito nella stessa soluzione 
    }
    return cnt;
}

int comb_rip(int pos, int *val, int *sol, int n, int k, int start, int cnt){
    int i; 
    if (pos >= k){
        for (i = 0; i <k; i ++){
            printf("%d", sol[i]);
        }
        printf("\n");
        return cnt +1;
    }
    for (i = start; i < n; i++){
        sol[pos] = val[i];
        cnt = comb_rip(pos+1, val, sol, n, k, i, cnt);
    }
    return cnt;
}

int powerset(int pos, int *val, int *sol, int n, int start, int cnt){
    int i;
    if (start >= n){
        for (i = 0; i < pos; i ++){
            printf("%d", sol[i]);
        }
        printf("\n");
        return cnt+1;
    }
    for (i = start; i < n; i++){
        sol[pos] = val[i];
        cnt = powerset(pos+1, val, sol, n, i+1, cnt);
    }
    cnt = powerset(pos, val, sol, n, n, cnt);
    return cnt;
}

int powerset_disp(int pos, int *val, int *sol, int n, int cnt){
    int i;
    if (pos>=n){
        printf("{ ");
        for (i = 0;i <n; i++){
            if (sol[i] != 0) printf("%d ", val[i]);
        }
        printf("}\n");
        return cnt+1;
    }
    sol[pos] = 0;
    cnt = powerset_disp(pos+1, val, sol, n, cnt);
    sol[pos] = 1;
    cnt = powerset_disp(pos+1, val, sol, n, cnt);
    return cnt;
}

int powerset_r (int *val, int n, int*sol, int k, int pos, int start){
    int cnt = 0, i;
    if(pos >= k){
        printf("\n{");
        for (i = 0; i < k; i++){
            printf("%d", sol[i]);
        }
        printf("}");
        return 1;
    }
    for (i = start; i < n; i++){
        sol[pos] = val[i];
        cnt += powerset_r(val, n, sol, k, pos+1, i+1);
    }
    return cnt+1;
}
int powerset_wrapper(int *val, int n, int *sol){
    int cnt = 0, k;
    printf("{}");
    cnt++;
    for (k=1; k<=n; k++){
        cnt+=powerset_r(val, n, sol, k, 0, 0);
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
    printf("ci sono %d permutazioni di %d elementi\n", cnt, n);


    // comb semplici 
    free(sol);
    sol = malloc(k*sizeof(int));
    cnt = comb_semplici(0, val, sol, n, k, 0, 0);
    printf("\nCi sono %d combinazioni semplici di %d elementi in classe %d\n", cnt, n, k);

    // comb rip
    cnt = comb_rip(0, val, sol, n, k, 0, 0);
    printf("\nCi sono %d combinazioni ripetute di %d elementi in classe %d\n", cnt, n, k);
    

    // powerset
    cnt = powerset(0, val, sol, n, 0, 0);
    printf("\nL'insieme delle parti contiene %d elementi\n", cnt);
    cnt = powerset_disp(0, val, sol, n, 0);
    cnt = powerset_wrapper(val, n, sol);

    return 0;
}