#include <stdio.h>

int factorial(int n){
    if (n == 0 || n == 1){
        return 1;
    }
    return n * factorial(n-1);
}

int main(){
    int n = 5;
    int x = factorial(n);
    printf( "%d", x);
}