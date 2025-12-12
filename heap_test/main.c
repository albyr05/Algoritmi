#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
typedef enum{false, true} bool;

int main(int argc, char* argv[]){
    Heap h = heap_init(15);
    for (int i = 0; i < 10; i++){
        heap_fill(h, i);
    }
    printf("Starting array: \n");
    heap_display(h);

    printf("\nMake it an heap: ");
    heap_build(h);
    heap_display(h);

    printf("\n Let's sort it: ");
    heap_sort(h);
    heap_display(h);

    heap_free(h);

    printf("\n");
    return 0;
}
/*
void stampaTruth (int n){
    printf("Questa è tutta la verità:\n");
    for (int i = 0; i < n; i++){

    }
}
*/

