#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pgList.h"
#include "invArray.h"
#include "pg.h"

#define N_SCELTE 7
#define DBG 0

enum { falso, vero };
typedef int bool;

void stampaMenu(char *opzioni[], int *selezioneUtente){
  int indice = 0;
  printf("\nMENU'\n");
  for (indice = 0; indice < N_SCELTE; indice++)
    printf("%2d > %s\n", indice, opzioni[indice]);
  scanf(" %d", selezioneUtente);
}

int main(int argc, char **argv) {
  char *opzioni[] = {
    "Uscita",
    "Stampa personaggi",
    "Stampa inventario",
    "Cerca personaggio",
    "Aggiungi personaggio",
    "Elimina personaggio",
    "Modifica equip"
  };

  char stringaCodice[LEN];
  int selezioneUtente;
  FILE *fileInput;
  bool terminaProgramma;

  pgList_t listaPg = pgList_init();
  invArray_t arrayInv = invArray_init();
  pg_t *puntatorePg, personaggioTemp;

  fileInput = fopen("pg.txt","r");
  pgList_read(fileInput, listaPg);
  fclose(fileInput);
#if DBG
  pgList_print(stdout, listaPg, arrayInv);
#endif /* DBG */

  fileInput = fopen("inventario.txt","r");
  invArray_read(fileInput, arrayInv);
  fclose(fileInput);
#if DBG
  invArray_print(stdout, arrayInv);
#endif /* DBG */

  terminaProgramma = falso;

  do {
    stampaMenu(opzioni, &selezioneUtente);
    switch (selezioneUtente) {

    case 0: {
      terminaProgramma = vero;
    } break;

    case 1: {
      pgList_print(stdout, listaPg, arrayInv);
    } break;

    case 2: {
      invArray_print(stdout, arrayInv);
    } break;

    case 3: {
      printf("Inserire codice personaggio: ");
      scanf("%s", stringaCodice);
      puntatorePg = pgList_searchByCode(listaPg, stringaCodice);
      if (puntatorePg != NULL) {
        pg_print(stdout, puntatorePg, arrayInv);
      }
    } break;

    case 4: {
      printf("Cod Nome Classe HP MP ATK DEF MAG SPR: ");
      if (pg_read(stdin, &personaggioTemp) != 0) {
        pgList_insert(listaPg, personaggioTemp);
      }
    } break;

    case 5: {
      printf("Inserire codice personaggio: ");
      scanf("%s", stringaCodice);
      pgList_remove(listaPg, stringaCodice);
    } break;

    case 6: {
      printf("Inserire codice personaggio: ");
      scanf("%s", stringaCodice);
      puntatorePg = pgList_searchByCode(listaPg, stringaCodice);
      if (puntatorePg != NULL) {
        pg_updateEquip(puntatorePg, arrayInv);
      }
    } break;

    default: {
      printf("Scelta non valida\n");
    } break;
    }
  } while (!terminaProgramma);

  pgList_free(listaPg);
  invArray_free(arrayInv);

  return 0;
}