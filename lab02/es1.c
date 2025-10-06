#include <stdio.h>

int gcd(int a, int b);

int main(){
    int a = 25, b = 90;
    int m = gcd(a, b);
    printf("%d", m);
    return 0;
}

int gcd (int a, int b){
    if (a < b){
        return gcd(b, a);           // swapping numbers 
    }
    if (a % b == 0){            // base case (not so sure)
        return b;
    }
    if (a % 2 == 0 && b % 2 == 0){                  // other cases as defined
        return 2*gcd(a/2, b/2);
    }
    if (a % 2 != 0 && b % 2 == 0){
        return gcd(a, b/2);
    }
    if (a % 2 == 0 && b % 2 != 0){
        return gcd (a/2, b);
    }
    else return gcd ((a-b)/2, b);
}