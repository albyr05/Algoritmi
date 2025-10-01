#include <stdio.h>

int mgc(int a, int b){
    if (a % b == 0){
        return b;
    }
    return (mgc(b, a%b));
}

int potenze(int a, int b){
    if (b == 1){
        return a;
    }
    return a * potenze(a, b-1);
}

int cerca(int v[], int n, int l, int r){
    if (l==r){
        if (v[l] == n){
            return 1;
        }
        return 0;
    }
    int m = (l+r)/2;
    int x = cerca (v, n, l, m);
    int y = cerca (v, n, m+1, r);
    if (x == 1 || y == 1){
        return 1;
    }
    return 0;

}
int main(){
    int a = 3, b = 3;
    int d = mgc(a, b);
    int p = potenze(a, b);
    int n = 10;
    int v[5] = {0, 1, 2, 3, 4};
    printf("Il massimo comune divisore tra %d e %d è %d\n", a, b, d);
    printf("Il risultato di %d alla %d é %d\n", a, b, p);
    if (cerca(v, n, 0, 4) == 1){
        printf("Il numero %d è presente nel vettore ", n);

    }
    else if (cerca(v, n, 0, 4) == 0){
        printf("Il numero %d non  è presente nel vettore ", n);
        
    }
    return 0;
}