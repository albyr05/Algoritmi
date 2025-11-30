#include "invArray.h"

#define MAX_INV 50

/* Struttura interna ADT */
struct invArray_s {
    inv_t vettoreInv[MAX_INV];
    int numeroInv;
};

/* Creazione ADT */
invArray_t invArray_init() {
    invArray_t arrayInv = (invArray_t)malloc(sizeof(struct invArray_s));
    if (arrayInv == NULL) return NULL;
    
    arrayInv->numeroInv = 0;
    return arrayInv;
}

/* Distruzione ADT */
void invArray_free(invArray_t arrayInv) {
    if (arrayInv != NULL) {
        free(arrayInv);
    }
}

/* Lettura da file */
void invArray_read(FILE *filePtr, invArray_t arrayInv) {
    if (arrayInv == NULL || filePtr == NULL) return;
    
    int numeroOggetti;
    fscanf(filePtr, "%d", &numeroOggetti);
    
    int indice;
    for (indice = 0; indice < numeroOggetti && indice < MAX_INV; indice++) {
        inv_read(filePtr, &arrayInv->vettoreInv[indice]);
        arrayInv->numeroInv++;
    }
}

/* Stampa inventario */
void invArray_print(FILE *filePtr, invArray_t arrayInv) {
    if (arrayInv == NULL || filePtr == NULL) return;
    
    fprintf(filePtr, "\n=== INVENTARIO (%d oggetti) ===\n", arrayInv->numeroInv);
    
    if (arrayInv->numeroInv == 0) {
        fprintf(filePtr, "(inventario vuoto)\n");
        return;
    }
    
    int indice;
    for (indice = 0; indice < arrayInv->numeroInv; indice++) {
        fprintf(filePtr, "[%d] ", indice);
        inv_print(filePtr, &arrayInv->vettoreInv[indice]);
    }
}

/* Stampa oggetto per indice */
void invArray_printByIndex(FILE *filePtr, invArray_t arrayInv, int posizione) {
    if (arrayInv == NULL || filePtr == NULL) return;
    if (posizione < 0 || posizione >= arrayInv->numeroInv) {
        fprintf(filePtr, "(indice non valido)\n");
        return;
    }
    
    inv_print(filePtr, &arrayInv->vettoreInv[posizione]);
}

/* Ritorna puntatore oggetto per indice */
inv_t *invArray_getByIndex(invArray_t arrayInv, int posizione) {
    if (arrayInv == NULL) return NULL;
    if (posizione < 0 || posizione >= arrayInv->numeroInv) return NULL;
    
    return &arrayInv->vettoreInv[posizione];
}

/* Ricerca oggetto per nome */
int invArray_searchByName(invArray_t arrayInv, char *nomeOggetto) {
    if (arrayInv == NULL || nomeOggetto == NULL) return -1;
    
    int indice;
    for (indice = 0; indice < arrayInv->numeroInv; indice++) {
        if (strcmp(arrayInv->vettoreInv[indice].nome, nomeOggetto) == 0) {
            return indice;
        }
    }
    
    return -1;
}