#ifndef PG_H
#define PG_H

#include "inv.h"

// ---Statistiche
typedef int stats_t[6];

// --- Equipaggiamento ---
typedef struct {
    ogg_t *slot[8];
    int n;
} equip_t;

// --- Personaggio ---
typedef struct {
    char codice[10];
    char name[50];
    char classe[50];
    stats_t stat;
    equip_t equipment;
} pg_t;

// --- Nodo di lista ---
typedef struct node_t node_t, *link;
struct node_t {
    pg_t val;     
    link next;
};

// --- Lista PG wrapper ---
typedef struct {
    link head;
    link tail;
    int n;
} list_pg_w;

// --- Funzioni ---

void list_init(list_pg_w *l);
void pg_list_load(list_pg_w *l, char *filename);
void insert_pg_from_user (list_pg_w *l);
void pg_list_insert(list_pg_w *l, pg_t new_pg);
void pg_list_remove(list_pg_w *l, char *code);
pg_t *search_pg_in_list(list_pg_w *l, char *code); // to do
void free_pg_list(list_pg_w *l);


void print_pg_details(pg_t *pg); // to do
void pg_update_equip(pg_t *pg, inventory_t *inv);   // to do

#endif