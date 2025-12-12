#include<stdio.h>
#include<stdlib.h>
#include "item.h"
typedef enum{false, true} bool;

int KEYget (Item item){
    return item.prio;
}

int KEYgmp (Item a, Item b) {return (KEYget(a) > KEYget(b)); }

void print_item(Item item){
    print("%s %d\n", item.name, item.prio );
}