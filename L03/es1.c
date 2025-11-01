#include <stdio.h>

int majority(int *a, int N);

int main (){
    int A[3] = {3, 3, 2};
    int major = majority(A, 3);
    if (major != -1){
        printf("L'elemento maggioritario del vettore è %d", major);
    }
    else {
        printf ("Il vettore non ha elemento maggioritario");
    }
}


int majority (int *a, int N){
    
    if (N==1){
        return *a;      // if the vector's length is equal to 1 return the value itself
    }
    int mid = (N+1)/2;
    int x = majority (a, mid);      // result of left vector 
    int y =  majority (a+mid, N-mid);         // result of right vector considering both odd and even sizes
    int conta_x = 0, conta_y = 0;       // starting the counters

    if (x==y){                      // if x == y the results are the same so i can return one of them 
        return x;
    }

    for(int i = 0; i<N; i++){           // checking the major in a bigger vector and calculating the counters
        if (*(a+i) == x) conta_x ++;
        else if (*(a+i) == y) conta_y ++;
    }

    if (conta_x > N/2){            // returning the majority of each sub-vector
        return x;
    }
    else if (conta_y > N/2){
        return y;
    }
    
    return -1;      // return -1 if no majority was found
}