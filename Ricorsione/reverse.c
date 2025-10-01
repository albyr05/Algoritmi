#include <stdio.h>
#include <string.h>

void rev(char v[], int l, int r){
    if (l==r){
        printf ("%c", v[l]);
        return;
    }
    int m = (l+r)/2;
    rev(v, m+1, r);
    rev(v, l, m);
    return;
}

int calcolacifre(int n){
    if (n <10){
        return 1;
    }
    return 1 + calcolacifre(n/10);
}

int main (){
    char v[] = "balordissimo";
    int n = 12345;
    rev(v, 0, strlen(v));
    int counter = calcolacifre(n);
    printf ("\nil numero %d contiene %d cifre", n, counter );
    return 0;
}