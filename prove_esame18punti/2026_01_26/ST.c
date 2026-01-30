#include "ST.h"

// MODULO CHE CONTIENE LE FUNZIONI DELL'ADT ST VISTE A LEZIONE 
struct symboltable {
    Key *v;
    int size;
    int max_size;
};

// alloco la symbol table dato un Max_size che è il numero di vertici del DAG, letto da file
ST ST_init(int max_size) {
    ST st = malloc(sizeof(*st));
    st->v = malloc(max_size * sizeof(Key));
    st->size = 0;
    st->max_size = max_size;
    return st;
}


int key_cmp(Key v1, Key v2) {
    int res = strcmp(v1.nome, v2.nome);   // confronto in primis i nomi 
    if (res != 0) return res;           // se hanno il nome diverso ritorno direttamente il confronto dei nomi 
    
    return strcmp(v1.d.born, v2.d.born);          // se hanno nome uguale passo al confronto della data di nascita 
}

// funzione per inserire una nuova chiave 
int ST_insert(ST st, Key val) {
    if (st->size >= st->max_size) return -1;        // non posso più inserire elementi 
    st->v[st->size] = val;          // inserisco il nuovo elemento
    return st->size++;          // returno direttamente la size incrementata, così evito di fare una ricerca dopo
}

int ST_search(ST st, Key val) {
    int i; 
    for (i = 0; i < st->size; i++) {            // controllo tutti gli elementi nell'ST
        if (key_cmp(st->v[i], val) == 0) {      
            return i;           // returno l'indice della ST
        }
    }
    return -1;      // non trovato nessuna corrsispondenza
}

Key key_set_void (){
    Date date = {"", ""};
    Key empty = {"", date};     // sono out of bound e quindi returno un item vuoto
    return empty;
}

Key ST_search_by_index(ST st, int id) {
    if (id < 0 || id >= st->size) {
        return key_set_void();
    }
    return st->v[id];
}


// free dell'ST
void ST_free(ST st) {
    free(st->v);
    free(st);
}