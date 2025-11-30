#include "inv.h"

/* Lettura statistiche da file */
void stat_read(FILE *filePtr, stat_t *puntStat) {
    fscanf(filePtr, "%d %d %d %d %d %d", 
           &puntStat->hp, &puntStat->mp, &puntStat->atk, 
           &puntStat->def, &puntStat->mag, &puntStat->spr);
}

/* Stampa statistiche con soglia minima */
void stat_print(FILE *filePtr, stat_t *puntStat, int sogliaMinima) {
    fprintf(filePtr, "%d %d %d %d %d %d",
            (puntStat->hp < sogliaMinima) ? sogliaMinima : puntStat->hp,
            (puntStat->mp < sogliaMinima) ? sogliaMinima : puntStat->mp,
            (puntStat->atk < sogliaMinima) ? sogliaMinima : puntStat->atk,
            (puntStat->def < sogliaMinima) ? sogliaMinima : puntStat->def,
            (puntStat->mag < sogliaMinima) ? sogliaMinima : puntStat->mag,
            (puntStat->spr < sogliaMinima) ? sogliaMinima : puntStat->spr);
}

/* Lettura oggetto da file */
void inv_read(FILE *filePtr, inv_t *puntInv) {
    fscanf(filePtr, "%s %s", puntInv->nome, puntInv->tipo);
    stat_read(filePtr, &puntInv->stat);
}

/* Stampa oggetto */
void inv_print(FILE *filePtr, inv_t *puntInv) {
    fprintf(filePtr, "%s %s ", puntInv->nome, puntInv->tipo);
    stat_print(filePtr, &puntInv->stat, 0); /* no soglia per oggetti */
    fprintf(filePtr, "\n");
}

/* Ritorna le statistiche di un oggetto */
stat_t inv_getStat(inv_t *puntInv) {
    return puntInv->stat;
}