#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"

#define MAX_S 60
int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    Graph G = readfile(argv[1]);
    int choice; bool flag = true;
    while (flag){
        printf("0. Print in alphabetic order the graph\n1. Memorize the graph with adjacency list\n2. Check adjacency of three given vertex\n");
        scanf (" %d", &choice);
        switch (choice){
            case 0:{
                print_order_wrap(G);
                break;
            }
            case 1:{
                create_ladj_w(G);
                break;
            }
            case 2:{
                printf("\nInsert the name of three vertex:");
                char label1[MAX_S], label2[MAX_S], label3[MAX_S];
                printf("\n1: ");
                scanf(" %s", label1);
                printf("\n2: ");
                scanf(" %s", label2);
                printf("\n3: ");
                scanf(" %s", label3);
                option o;
                printf("Wanna use adjacency matrix [0] or adjacency list[1]");
                scanf(" %d", &o);
                if (check_adj_w(label1, label2, label3, G, o)){
                    printf ("\nThe nodes are adjacent two by two\n");
                }
                else printf("\nThe nodes are not adjacent\n");
                break;
            }
            default:{
                flag = false;
                break;
            }
        }
    }
    printf("\n");
    return 0;
}