#include "pgList.h"

/* Nodo della lista */
typedef struct pgNode_s {
    pg_t personaggio;
    struct pgNode_s *successivo;
} pgNode_t;

/* Struttura interna ADT */
struct pgList_s {
    pgNode_t *testa;
    int numeroPg;
};

/* Creazione ADT */
pgList_t pgList_init() {
    pgList_t listaPg = (pgList_t)malloc(sizeof(struct pgList_s));
    if (listaPg == NULL) return NULL;
    
    listaPg->testa = NULL;
    listaPg->numeroPg = 0;
    return listaPg;
}

/* Distruzione ADT */
void pgList_free(pgList_t listaPg) {
    if (listaPg == NULL) return;
    
    pgNode_t *corrente = listaPg->testa;
    pgNode_t *temporaneo;
    
    while (corrente != NULL) {
        temporaneo = corrente;
        corrente = corrente->successivo;
        pg_clean(&temporaneo->personaggio);
        free(temporaneo);
    }
    
    free(listaPg);
}

/* Lettura da file */
void pgList_read(FILE *filePtr, pgList_t listaPg) {
    if (listaPg == NULL || filePtr == NULL) return;
    
    pg_t pgTemp;
    
    /* Leggi personaggi fino a EOF */
    while (pg_read(filePtr, &pgTemp)) {
        pgList_insert(listaPg, pgTemp);
    }
}

/* Stampa lista personaggi */
void pgList_print(FILE *filePtr, pgList_t listaPg, invArray_t arrayInv) {
    if (listaPg == NULL || filePtr == NULL) return;
    
    fprintf(filePtr, "\n=== LISTA PERSONAGGI (%d personaggi) ===\n", listaPg->numeroPg);
    
    if (listaPg->numeroPg == 0) {
        fprintf(filePtr, "(lista vuota)\n");
        return;
    }
    
    pgNode_t *corrente = listaPg->testa;
    int contatore = 1;
    
    while (corrente != NULL) {
        fprintf(filePtr, "\n--- Personaggio %d ---\n", contatore++);
        pg_print(filePtr, &corrente->personaggio, arrayInv);
        corrente = corrente->successivo;
    }
}

/* Inserimento in testa */
void pgList_insert(pgList_t listaPg, pg_t personaggio) {
    if (listaPg == NULL) return;
    
    pgNode_t *nuovoNodo = (pgNode_t *)malloc(sizeof(pgNode_t));
    if (nuovoNodo == NULL) return;
    
    nuovoNodo->personaggio = personaggio;
    nuovoNodo->successivo = listaPg->testa;
    listaPg->testa = nuovoNodo;
    listaPg->numeroPg++;
}

/* Rimozione per codice */
void pgList_remove(pgList_t listaPg, char *codice) {
    if (listaPg == NULL || codice == NULL) return;
    if (listaPg->testa == NULL) return;
    
    pgNode_t *corrente = listaPg->testa;
    pgNode_t *precedente = NULL;
    
    while (corrente != NULL) {
        if (strcmp(corrente->personaggio.cod, codice) == 0) {
            /* Trovato, rimuovi */
            if (precedente == NULL) {
                /* Primo nodo */
                listaPg->testa = corrente->successivo;
            } else {
                precedente->successivo = corrente->successivo;
            }
            
            pg_clean(&corrente->personaggio);
            free(corrente);
            listaPg->numeroPg--;
            printf("Personaggio %s rimosso con successo.\n", codice);
            return;
        }
        
        precedente = corrente;
        corrente = corrente->successivo;
    }
    
    printf("Personaggio con codice %s non trovato.\n", codice);
}

/* Ricerca per codice */
pg_t *pgList_searchByCode(pgList_t listaPg, char *codice) {
    if (listaPg == NULL || codice == NULL) return NULL;
    
    pgNode_t *corrente = listaPg->testa;
    
    while (corrente != NULL) {
        if (strcmp(corrente->personaggio.cod, codice) == 0) {
            return &corrente->personaggio;
        }
        corrente = corrente->successivo;
    }
    
    return NULL;
}