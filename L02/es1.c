#include <stdio.h>

int gcd(int a, int b);

int main(){
    int a = 25, b = 90;
    int m = gcd(a, b);
    printf("%d", m);
    return 0;
}

int gcd(int a, int b) {
    if (a == b) return a;        // caso base: numeri uguali
    if (a == 0) return b;        // caso base: uno dei due è zero
    if (b == 0) return a;

    if (a % 2 == 0 && b % 2 == 0)        // entrambi pari
        return 2 * gcd(a / 2, b / 2);
    else if (a % 2 == 0)                 // solo a pari
        return gcd(a / 2, b);
    else if (b % 2 == 0)                 // solo b pari
        return gcd(a, b / 2);
    else if (a > b)                      // entrambi dispari, a > b
        return gcd((a - b) / 2, b);
    else                                 // entrambi dispari, b > a
        return gcd((b - a) / 2, a);
}