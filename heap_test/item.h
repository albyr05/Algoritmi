#ifndef ITEM_H
#define ITEM_H

typedef struct{
    char name[20];
    int prio;
} Item;

int KEYget (Item item);
int KEYcmp (Item a, Item b);
void print_item(Item);
#endif