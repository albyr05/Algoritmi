#include "pg.h"

/* Lettura personaggio da file (ritorna 1 se successo, 0 altrimenti) */
int pg_read(FILE *filePtr, pg_t *puntPg) {
    if (filePtr == NULL || puntPg == NULL) return 0;
    
    if (fscanf(filePtr, "%s %s %s", puntPg->cod, puntPg->nome, puntPg->classe) != 3) {
        return 0;
    }
    
    stat_read(filePtr, &puntPg->b_stat);
    
    /* Inizializza statistiche equipaggiamento a zero */
    puntPg->eq_stat.hp = 0;
    puntPg->eq_stat.mp = 0;
    puntPg->eq_stat.atk = 0;
    puntPg->eq_stat.def = 0;
    puntPg->eq_stat.mag = 0;
    puntPg->eq_stat.spr = 0;
    
    /* Crea equipaggiamento vuoto */
    puntPg->equip = equipArray_init();
    
    return 1;
}

/* Pulizia personaggio (dealloca equipaggiamento) */
void pg_clean(pg_t *puntPg) {
    if (puntPg == NULL) return;
    if (puntPg->equip != NULL) {
        equipArray_free(puntPg->equip);
        puntPg->equip = NULL;
    }
}

/* Calcola statistiche totali (base + equipaggiamento) */
static void calcolaStatistiche(pg_t *puntPg, invArray_t arrayInv) {
    if (puntPg == NULL || arrayInv == NULL) return;
    
    /* Reset statistiche equipaggiamento */
    puntPg->eq_stat.hp = 0;
    puntPg->eq_stat.mp = 0;
    puntPg->eq_stat.atk = 0;
    puntPg->eq_stat.def = 0;
    puntPg->eq_stat.mag = 0;
    puntPg->eq_stat.spr = 0;
    
    /* Somma i modificatori di ogni oggetto equipaggiato */
    int indice;
    for (indice = 0; indice < EQUIP_SLOT; indice++) {
        int indiceOggetto = equipArray_getEquipByIndex(puntPg->equip, indice);
        if (indiceOggetto != -1) {
            inv_t *oggetto = invArray_getByIndex(arrayInv, indiceOggetto);
            if (oggetto != NULL) {
                stat_t statOggetto = inv_getStat(oggetto);
                puntPg->eq_stat.hp += statOggetto.hp;
                puntPg->eq_stat.mp += statOggetto.mp;
                puntPg->eq_stat.atk += statOggetto.atk;
                puntPg->eq_stat.def += statOggetto.def;
                puntPg->eq_stat.mag += statOggetto.mag;
                puntPg->eq_stat.spr += statOggetto.spr;
            }
        }
    }
}

/* Stampa personaggio */
void pg_print(FILE *filePtr, pg_t *puntPg, invArray_t arrayInv) {
    if (filePtr == NULL || puntPg == NULL) return;
    
    fprintf(filePtr, "\n=== PERSONAGGIO ===\n");
    fprintf(filePtr, "Codice: %s\n", puntPg->cod);
    fprintf(filePtr, "Nome: %s\n", puntPg->nome);
    fprintf(filePtr, "Classe: %s\n", puntPg->classe);
    
    fprintf(filePtr, "Statistiche base: ");
    stat_print(filePtr, &puntPg->b_stat, 0);
    fprintf(filePtr, "\n");
    
    /* Calcola e stampa statistiche totali */
    calcolaStatistiche(puntPg, arrayInv);
    
    stat_t statTotali;
    statTotali.hp = puntPg->b_stat.hp + puntPg->eq_stat.hp;
    statTotali.mp = puntPg->b_stat.mp + puntPg->eq_stat.mp;
    statTotali.atk = puntPg->b_stat.atk + puntPg->eq_stat.atk;
    statTotali.def = puntPg->b_stat.def + puntPg->eq_stat.def;
    statTotali.mag = puntPg->b_stat.mag + puntPg->eq_stat.mag;
    statTotali.spr = puntPg->b_stat.spr + puntPg->eq_stat.spr;
    
    fprintf(filePtr, "Statistiche totali: ");
    stat_print(filePtr, &statTotali, MIN_STAT);  /* applica soglia minima */
    fprintf(filePtr, "\n");
    
    /* Stampa equipaggiamento */
    if (arrayInv != NULL) {
        equipArray_print(filePtr, puntPg->equip, arrayInv);
    }
}

/* Modifica equipaggiamento personaggio */
void pg_updateEquip(pg_t *puntPg, invArray_t arrayInv) {
    if (puntPg == NULL || arrayInv == NULL) return;
    equipArray_update(puntPg->equip, arrayInv);
}