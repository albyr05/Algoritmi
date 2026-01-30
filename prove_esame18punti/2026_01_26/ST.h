#ifndef ST_H
#define ST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXS 51

// le definizioni di data e key le metto nel .h del modulo Simble table in modo che siano quasi ADT
typedef struct {
    char born[11]; // formato aaaa/mm/gg
    char death[11];
} Date;

typedef struct {
    char nome[MAXS];
    Date d;
} Key;

typedef struct symboltable *ST;


ST ST_init(int max_size);
void ST_free(ST st);
int ST_insert(ST st, Key val);
int ST_search(ST st, Key val);
Key ST_search_by_index(ST st, int id);
int key_cmp(Key v1, Key v2);     // funzione di confronto richiesta

#endif