#include "equipArray.h"

/* Struttura interna ADT */
struct equipArray_s {
    int vettoreEquip[EQUIP_SLOT];  /* indici degli oggetti in inventario */
    int numeroEquip;               /* numero slot occupati */
};

/* Creazione ADT */
equipArray_t equipArray_init() {
    equipArray_t arrayEquip = (equipArray_t)malloc(sizeof(struct equipArray_s));
    if (arrayEquip == NULL) return NULL;
    
    arrayEquip->numeroEquip = 0;
    int indice;
    for (indice = 0; indice < EQUIP_SLOT; indice++) {
        arrayEquip->vettoreEquip[indice] = -1;  /* -1 = slot vuoto */
    }
    return arrayEquip;
}

/* Distruzione ADT */
void equipArray_free(equipArray_t arrayEquip) {
    if (arrayEquip != NULL) {
        free(arrayEquip);
    }
}

/* Quanti equipaggiamenti in uso */
int equipArray_inUse(equipArray_t arrayEquip) {
    if (arrayEquip == NULL) return 0;
    return arrayEquip->numeroEquip;
}

/* Stampa equipaggiamento */
void equipArray_print(FILE *filePtr, equipArray_t arrayEquip, invArray_t arrayInv) {
    if (arrayEquip == NULL || filePtr == NULL || arrayInv == NULL) return;
    
    fprintf(filePtr, "  Equipaggiamento (%d/%d slot):\n", arrayEquip->numeroEquip, EQUIP_SLOT);
    if (arrayEquip->numeroEquip == 0) {
        fprintf(filePtr, "    (nessun equipaggiamento)\n");
        return;
    }
    
    int indice;
    for (indice = 0; indice < EQUIP_SLOT; indice++) {
        if (arrayEquip->vettoreEquip[indice] != -1) {
            fprintf(filePtr, "    [%d] ", indice);
            invArray_printByIndex(filePtr, arrayInv, arrayEquip->vettoreEquip[indice]);
        }
    }
}

/* Modifica equipaggiamento */
void equipArray_update(equipArray_t arrayEquip, invArray_t arrayInv) {
    if (arrayEquip == NULL || arrayInv == NULL) return;
    
    int sceltaUtente, indiceInventario, slotEquip;
    
    printf("\n1. Aggiungi oggetto\n2. Rimuovi oggetto\nScelta: ");
    scanf("%d", &sceltaUtente);
    
    if (sceltaUtente == 1) {
        /* Aggiungi oggetto */
        if (arrayEquip->numeroEquip >= EQUIP_SLOT) {
            printf("Equipaggiamento pieno!\n");
            return;
        }
        
        printf("Inserisci indice oggetto da inventario: ");
        scanf("%d", &indiceInventario);
        
        /* Verifica indice valido */
        inv_t *oggetto = invArray_getByIndex(arrayInv, indiceInventario);
        if (oggetto == NULL) {
            printf("Indice non valido!\n");
            return;
        }
        
        /* Trova primo slot libero */
        int posizione;
        for (posizione = 0; posizione < EQUIP_SLOT; posizione++) {
            if (arrayEquip->vettoreEquip[posizione] == -1) {
                arrayEquip->vettoreEquip[posizione] = indiceInventario;
                arrayEquip->numeroEquip++;
                printf("Oggetto aggiunto allo slot %d\n", posizione);
                return;
            }
        }
    } else if (sceltaUtente == 2) {
        /* Rimuovi oggetto */
        if (arrayEquip->numeroEquip == 0) {
            printf("Equipaggiamento vuoto!\n");
            return;
        }
        
        printf("Inserisci slot da liberare (0-%d): ", EQUIP_SLOT - 1);
        scanf("%d", &slotEquip);
        
        if (slotEquip < 0 || slotEquip >= EQUIP_SLOT) {
            printf("Slot non valido!\n");
            return;
        }
        
        if (arrayEquip->vettoreEquip[slotEquip] == -1) {
            printf("Slot gia vuoto!\n");
            return;
        }
        
        arrayEquip->vettoreEquip[slotEquip] = -1;
        arrayEquip->numeroEquip--;
        printf("Oggetto rimosso dallo slot %d\n", slotEquip);
    }
}

/* Ritorna indice oggetto in posizione index dell'equipaggiamento */
int equipArray_getEquipByIndex(equipArray_t arrayEquip, int posizione) {
    if (arrayEquip == NULL) return -1;
    if (posizione < 0 || posizione >= EQUIP_SLOT) return -1;
    
    return arrayEquip->vettoreEquip[posizione];
}