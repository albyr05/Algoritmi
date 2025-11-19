#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "pg.h"
#include "inv.h"

typedef enum{false, true} bool;


void list_init(list_pg_w *l){
    l->head = NULL;
    l->tail = NULL;
    l->n = 0;
}


void pg_list_insert(list_pg_w *l, pg_t new_pg){
    link x = malloc(sizeof(node_t));
    if (x == NULL) return;
    x->next = NULL;
    x->val = new_pg;
    if (l->n == 0){
        l->head = x;
        l->tail = x;
    }
    else{
        l->tail->next = x;
        l ->tail = x;
    }
    l->n ++;
}
bool check_valid_code (list_pg_w *l, char *checking_code){
    link x;
    for (x = l->head; x != NULL; x = x->next){
        if (strcmp(checking_code, x->val.codice) == 0) return false;
    }
    return true;
}
void insert_pg_from_user(list_pg_w *l){
    pg_t help_pg;
    printf("\nInsert the unique code for the new pg: ");
    scanf(" %s", help_pg.codice);
    if (check_valid_code(l, help_pg.codice)){
        printf("\nInsert pg name: ");
        scanf(" %s", help_pg.name);
        printf("\nInsert pg class: ");
        scanf(" %s", help_pg.classe);
        printf("\nInsert pg base stats [hp mp atk def mag spr]: ");
        for (int i = 0; i < 6; i++){
            scanf(" %d", &help_pg.stat[i]);
        }
        pg_list_insert(l, help_pg);
    }
    else return;
}
bool key_eq(pg_t pg, char *code){
    if (strcmp(pg.codice, code) == 0){
        return true;
    }
    return false;
}

pg_t *search_pg_in_list(list_pg_w *l, char *code){
    link x;
    for (x = l->head; x != NULL; x = x -> next){
        if (key_eq(x->val, code)){
            return &x->val;
        }
    }
    return NULL;
}

void pg_list_remove(list_pg_w *l, char *code){
    if (l->head == NULL) {
        return; // lista vuota
    }
    link p = l->head, x = p->next;
    // caso: rimuovo la testa
    if (strcmp(p->val.codice, code) == 0){
        l->head = p->next;
        if (l->head == NULL) l->tail = NULL;
        free(p);
        return;
    }
    // caso generale
    while (x != NULL){
        if (strcmp(x->val.codice, code) == 0){
            p->next = x->next;
            if (x == l->tail) l->tail = p;
            free(x);
            return;
        }
        p = x;
        x = x->next;
    }
}

void pg_list_load(list_pg_w *l, char *filename){
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }
    pg_t help_pg;
    while(fscanf(fp, "%s %s %s", help_pg.codice, help_pg.name, help_pg.classe) == 3){
        for (int i = 0; i < 6; i++){
            fscanf(fp, "%d", &help_pg.stat[i]);
        }
        for (int i = 0; i < 8; i++){
            help_pg.equipment.slot[i] = NULL;
        }
        help_pg.equipment.n = 0;
        pg_list_insert(l, help_pg);
    }

    fclose(fp);
}

bool add_obj(pg_t *pg, inventory_t *inv, char *name_obj){
    int i;
    
    ogg_t *insert_ogg = search_ogg(inv, name_obj);
    if (insert_ogg == NULL) return false; 
    i = 0;
    if (pg->equipment.n == 7) return false;     // not enough space for another obj
    while (pg->equipment.slot[i] != NULL){     // adding in the first free slot
        i++;
    }
    pg->equipment.slot[i] = insert_ogg; pg->equipment.n ++;         // adding the object and incrementing the number of full slots
    for (int j = 0; j < 6; j++){
        pg->stat[j] += insert_ogg->bonus[j];        // adding the bonus of the insert obj
    }
    return true;
}

void print_pg_equip(pg_t *pg){
    for (int i = 0; i < 8; i++){
        printf("SLOT %d: ", i);
        if (pg->equipment.slot[i] == NULL) printf ("empty\n");
        else print_ogg_details(pg->equipment.slot[i]);
    }
}


// si prende il puntatore al personaggio ritornato dalla search_pg e chiede anche all'utente gli input degli oggetti
void pg_update_equip(pg_t *pg, inventory_t *inv){
    int choice; 
    printf("you want to add or remove an object for this pg? [0,1] ");
    scanf(" %d", &choice);
    
    switch (choice){
        case 0:{
            char name_obj[50];
            printf("This is the current inventory:\n");
            print_inventory(inv);
            printf("Insert the name of the object: ");
            scanf(" %s", name_obj);
            if (add_obj(pg, inv, name_obj)){
                printf ("Object succesfully added :)\n");
                
            }
            else{
                printf("Slots full or object not present in the inventory!\n");
            }
            break;
        }
        case 1: {
            int slot_n;
            printf ("This is the current equipment: \n");
            print_pg_equip(pg);
            printf("Insert the slot of the object to remove from the pg equipment: ");
            scanf (" %d", &slot_n);
            if (pg->equipment.slot[slot_n] == NULL) printf("\nthis slot is already empty!");
            else{
                for (int j = 0; j < 6; j++){
                    pg->stat[j] -= pg->equipment.slot[slot_n]->bonus[j];        // removing the bonus of the object
                }
                pg->equipment.slot[slot_n] = NULL; 
                pg->equipment.n --;
                
                printf("\nslot succesfully freed :)");
            }
            break;
        }
    }
}

void print_pg_details(pg_t *pg){
    printf("\n");
    printf("UNIQUE CODE: %s\n NAME: %s\n CLASS: %s  ",  pg->codice, pg->name, pg->classe);
    printf("PG STATS: ");
    for (int i = 0; i < 6; i++){
        if (pg->stat[i] >= 0) printf("%d ", pg->stat[i]);
        else printf("0 ");
    }
    printf("PG EQUIPMENT: \n");
    print_pg_equip(pg);
}


void free_pg_list(list_pg_w *l){
    link x, temp;
    for (x = l->head; x != NULL; x = temp) {
        temp = x->next;   // salvo il prossimo nodo
        free(x);          // libero il nodo corrente
    }
    l->head = l->tail = NULL;
    l->n = 0;
}