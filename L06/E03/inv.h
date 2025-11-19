#ifndef INV_H
#define INV_H


typedef struct {
    char name[50];
    char type[50];
    int bonus[6];   
} ogg_t;

typedef struct {
    ogg_t *v;  
    int n;      
} inventory_t;

void inventory_init(inventory_t *inv);
void load_inventory(inventory_t *inv, char *filename);

void print_inventory(inventory_t *inv);

ogg_t *search_ogg(inventory_t *inv, char *name);


void print_ogg_details(ogg_t *ogg);


void free_inventory(inventory_t *inv);

#endif