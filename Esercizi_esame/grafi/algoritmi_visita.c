#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


int main() {
    Graph G = read_file("input.txt");

    int scelta = 0;
    int v, w;

    do {
        printf("\n--- MENU GRAFO ---\n");
        printf("1) Graph path (cammino tra due vertici)\n");
        printf("2) DFS da un vertice\n");
        printf("3) Componenti connesse\n");
        printf("4) Ordinamento topologico inverso per DAG\n");
        printf("5) Componenti fortemente connesse del grafo\n");
        printf("0) Esci\n");
        printf("Scelta: ");

        scanf(" %d", &scelta);

        switch (scelta) {

        case 1:
            printf("Inserisci vertice di partenza: ");
            scanf(" %d", &v);
            printf("Inserisci vertice di arrivo: ");
            scanf(" %d", &w);

            graph_path(G, v, w);
            break;

        case 2:
            printf("Inserisci vertice di partenza DFS: ");
            scanf(" %d", &v);

            dfs_w(G, v);
            break;

        case 3:
            graphcc(G);
            break;

        case 4:
            dagrts(G);
            break;
        case 5:
            print_graph(G);
            graphscc(G);
            break;
        case 0:
            printf("Uscita dal programma\n");
            break;

        default:
            printf("Scelta non valida\n");
        }

    } while (scelta != 0);

    graph_free(G);
    return 0;
}