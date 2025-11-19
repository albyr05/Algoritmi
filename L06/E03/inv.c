#include<stdio.h>
#include<stdlib.h>
#include "inv.h"
#include <string.h>

typedef enum{false, true} bool;

void inventory_init(inventory_t *inv){
    inv->v = NULL;
    inv->n = 0;
}
void load_inventory(inventory_t *inv, char *filename){
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }
    
    int nr, i;
    fscanf(fp, "%d", &nr);
    inv->n = nr;
    inv->v = malloc((inv->n)*sizeof(ogg_t));
    for (i = 0; i < inv->n; i++){
        fscanf(fp, "%s %s", inv->v[i].name, inv->v[i].type);
        for (int j = 0; j < 6; j++){
            fscanf(fp, "%d", &inv->v[i].bonus[j]);
        }
    }

    fclose(fp);

}
void print_inventory(inventory_t *inv){
    for (int i = 0; i < inv->n; i++){
        printf("NAME %s, TYPE %s", inv->v[i].name, inv->v[i].type);
        printf("    BONUS:  ");
        for (int j = 0; j < 6; j++){
            printf("%d ", inv->v[i].bonus[j]);
        }
        printf("\n");
    }
    printf("\n\n");
}
ogg_t *search_ogg(inventory_t *inv, char *name){
    int i; 
    for (i = 0; i < inv->n; i ++){
        if (strcmp(name, inv->v[i].name) == 0){
            return &inv->v[i];
        }
    }
    return NULL;
}

void print_ogg_details(ogg_t *ogg){
    printf("OGG details: %s %s;     ", ogg->name, ogg->type);
    for (int i = 0; i < 6; i++){
        printf("BONUS: %d ", ogg->bonus[i]);
    } 
    printf("\n");
}

void free_inventory(inventory_t *inv){
    free(inv->v);
}

