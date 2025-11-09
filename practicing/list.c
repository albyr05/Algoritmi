#include <stdio.h>
#include <stdlib.h>

typedef enum{False, True} bool;
typedef struct n *link;
typedef struct n {
    int val;
    link next;
} nodo;

link new_node(int v, link next) {
    link x = malloc(sizeof(*x));
    if (x == NULL) {
        return NULL;
    }
    x->val = v;
    x->next = next;
    return x;
}

link insertHead(link h, int v) {
    return new_node(v, h);
}

link insertTail(link t, int v) {
    if (t == NULL) return new_node(v, NULL); // caso lista vuota
    t->next = new_node(v, NULL);
    return t->next; // restituisce il nuovo tail
}

int keyget(link x){
    return x-> val;         // prendo la chiave che mi serve del nodo x
}
bool keycmp (int val, int k){
    if (val == k){              // confronto il valore del nodo con la chiave di ricerca
        return True;
    }
    return False;
}

int setvoid(){
    return -1;  //se non trovo nessun ndoo ritorno un valore non valido
}

int listsearch(link h, int k){
    link x; 
    for (x = h; x != NULL; x = x->next){        // attraverso tutta la lista 
        if (keycmp(keyget(x), k)){      // confronto le 2 chiavi
            return x->val;          // returno il valore del nodo che mi serve 
        }   
    }
    return setvoid();      
}

link listheaddel (link h){
    link x = h;
    if (h == NULL){             // se la lista è vuota return NULL
        return NULL;
    }
    h = h->next;        // la nuova testa è il secondo elemento
    free(x);            // libero la vecchia testa 
    return h;
}

int listextrhead(link *hp){
    link x = *hp;
    int tmp;
    if (x==NULL){
        return setvoid();
    }
    tmp = x->val;
    *hp = x->next;
    free(x);
    return tmp;
}

link listDelkey (link h, int k){
    link x, p;
    if (h == NULL){
        return NULL;
    }
    for (x=h, p = NULL; x != NULL; p = x, x= x->next){
        if (keycmp(keyget(x), k)){
            if (x == h){
                h = x->next;
            }
            else p -> next = x -> next;
            free(x); break;
        }

    }
    return h;
}

void printlist (link h){
    printf("\n");
    for (link x = h; x != NULL; x = x->next)
        printf("%d -> ", x->val);
    printf("NULL\n");
}


int main() {
    link h = NULL;
    link t = NULL;

    // Inserisco il primo nodo
    h = insertHead(NULL, 0);
    t = h;  // head e tail coincidono all'inizio

    // Aggiungo 5 nodi in coda
    for (int i = 1; i < 5; i++){
        t = insertTail(t, i);
    }
    printlist(h);
    int search_value = 3;
    int s = listsearch(h, search_value);
    if (s != -1){
        printf ("é stato trovato il valore %d\n", s);
    }
    else printf("Non è stato trovato il valore %d \n", search_value);

    h = listheaddel(h); // cancello la testa
    printlist(h);
    int extract = listextrhead(&h);
    printf("Il valore estratto %d", extract);
    printlist(h);
    h = listDelkey(h, 4);
    printlist(h);

    return 0;

}