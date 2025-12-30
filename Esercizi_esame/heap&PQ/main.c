#include<stdio.h>
#include<stdlib.h>
#include "heap.h"
#include "pq.h"
typedef enum{false, true} bool;

int main(int argc, char* argv[]){
    int choice;
    printf ("what do yo wanna do: heap[0]/PQ[1]: ");
    scanf(" %d", &choice);
    switch(choice){
        case 0: {
            int n;
            printf("Insert the number of element: ");
            scanf (" %d", &n);
            h heap = heap_init(n);
            printf ("Insert the vector: ");
            heap_read(heap, n);
            heapbuild(heap);
            printf ("HEAP: \n");
            print_heap(heap);
            printf("\nSTEP OF HEAP_SORT:\n ");
            heap_sort(heap);
            printf("\n");
            break;
        }
        case 1:{
            int maxn;
            printf ("insert the number of max element: ");
            scanf(" %d", &maxn);
            PQ pq = pq_init(maxn);
            int val;
            printf("Insert the elements: \n");
            for (int i = 0; i < maxn; i++){
                scanf (" %d", &val);
                pq_insert(pq, val);
            }
            printf("COMPLETE QUEUE: \n");
            pq_display(pq);

/*
            int extracted_val = pq_extract_max(pq);
            printf("\nElement with max priority %d\n", extracted_val);
            printf("COMPLETE QUEUE: \n");
            pq_display(pq);
            */


            int old_val, new_val;
            printf("\nInsert the old val and the new one");
            scanf(" %d %d", &old_val, &new_val);
            pq_change(pq, new_val, old_val);
            printf("COMPLETE QUEUE: \n");
            pq_display(pq);
        }


    }
}