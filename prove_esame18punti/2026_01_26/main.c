#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "ST.h"

// parametro k passato come terzo argomento al main, file di input, come secondo 
int main(int argc, char *argv[]) {

    if (argc != 3) return -1;

    int scelta;

    int k = atoi(argv[2]);
    
    
    GRAPH G = graph_load(argv[1]);

    if (G == NULL) {
        printf("Errore\n");
        return -1;
    }
    
    do {
        printf("\n\nMENU\n");
        printf("1. Verifica relazione di parentela (relationshipK)\n");
        printf("2. Trova persona con più discendenti (largestLineageK)\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        
        switch(scelta) {
            case 1: {
                //RELATIONSHIPK
                char nome1[MAXS], nome2[MAXS];
                Date d1, d2;
                
                
                printf("Prima persona:\n NOME: ");
                scanf("%s", nome1);
                printf("    Data di nascita (aaaa/mm/gg): ");
                scanf(" %s", d1.born);
                printf("    Data di morte (aaaa/mm/gg o 0/0/0 se in vita): ");
                scanf("%s", d1.death);
                

                // lettura dati seconda persona
                printf("Seconda persona:\n  NOME: ");
                
                scanf("%s", nome2);
                printf("    Data di nascita (aaaa/mm/gg): ");
                scanf("%s", d2.born);
                printf("    Data di morte (aaaa/mm/gg o 0/0/0 se in vita): ");
                scanf("%s", d2.death);
                
                
                if (relationshipK(G, nome1, d1, nome2, d2, k)) {
                    printf("\nLe due persone sono parenti di grado <= %d\n", k);
                } 
                else {
                    printf("\n Le due persone non sono parenti di grado <= %d\n", k);
                }
                break;
            }
            
            // LARGESTLINEAGEK
            case 2: {
                largestLineageK(G, k);
                break;
            }

            case 0:
                break;
                
            default:
                printf("\nScelta non valida!\n");
        }
        
    } while(scelta != 0);
    
   // libero il grafo
    graph_free(G);
    
    return 0;
}