#include <stdio.h>
int max (int l, int r, int v[]);

int max (int l, int r, int v[]){
    if (l == r){
        return v[l];
    }
    int m = (l+r) / 2;
    int w = max (l, m, v);
    int u = max(m+1, r, v);
    if (w > u) {
        return w;
    };
    return u;
}

int main(){
    int v[8] = {2, 4, 1, 5, 7, 100, 3, 8};
    int massimo = max(0, 7, v);
    printf ("Il massimo è: %d", massimo);
    return 0;
}