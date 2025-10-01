#include <stdio.h>

void hanoi(int n, int src, int dest){
    int aux = 3 - (src+dest);
    if (n==1) return;
    hanoi (n-1, src, aux);
    hanoi (n-1, aux, dest);
    return;
}

int main (){
    hanoi (32, 0, 31);
    printf("ciao");
    return 0;
}