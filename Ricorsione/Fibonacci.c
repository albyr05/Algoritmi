#include <stdio.h>                  

int fib(int n);

int fib (int n){
    if (n == 0 || n == 1){
        return n;
    }
    int n2 = fib(n-2);
    int n1 = fib(n-1);
    return n1 + n2;
}

int main(){
    int x = fib(4);
    printf("%d", x);
}
