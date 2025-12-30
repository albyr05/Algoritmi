#ifndef ST_H
#define ST_H

typedef struct item *Item;

struct item {
    
};

typedef struct symbletable *ST;

ST STinit(int MAXN);
int STinsert(ST st, Item item); //inserisce nella prima posizione disponibile "val" solo se val non è presente - ritorna l'indice al quale è stato inserito - se non ho abbastanza spazio già allocato usa una realloc per allocare il doppio rispetto alla memoria attualemente presente.
int STgetIndex(ST st, char* key); //ritorna la posizione in cui è presente "val", altrimenti ritorna -1
Item STget(ST st, int index);
Item STsearch(ST st, char *key);
int STcount(ST st);
void STprint(ST st);
void STfree(ST st); //libera la memoria

#endif