#include<stdio.h>
#include<stdlib.h>
#include "graph.h"
#include "queue.h"
#include "ST.h"


typedef struct node *link;

struct node{
    int w;
    link next;
};


struct graph{
    int V;
    ST st;
    link *ladj;
    link *ladj_rev;         // aggiungo una lista delle adiacenze dove gli archi vanno da figli a genitori così ho il grafo trasposto e posso fare la BFS dal basso verso l'alto
};


// Funzione per creare un nuovo nodo della lista (aggiunta in testa)
link new_node(int w, link next) {
    link x = malloc(sizeof(*x));
    x->w = w;
    x->next = next;
    return x;
}

// Inizializza il grafo 
GRAPH graph_init(int V) {
    GRAPH G = malloc(sizeof(*G));
    G->V = V;
    G->st = ST_init(V);
    G->ladj = calloc(V, sizeof(link));
    G->ladj_rev = calloc(V, sizeof(link));
    return G;
}

// IMPLEMENTAZIONE DELLA GRAPH LOAD AGGIUNTA PER FAR ESEGUIRE IL PROGRAMMA
// IL FORMATO DEL FILE DEVE ESSERE COME QUELLO D'ESEMPIO NEL FILE INPUT.TXT
GRAPH graph_load(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore %s\n", filename);
        return NULL;
    }

    int V;
    fscanf(fp, "%d", &V);
    GRAPH G = graph_init(V);

    // leggo i vertici
    for (int i = 0; i < V; i++) {
        Key person;
        fscanf(fp, "%s %s %s", person.nome, person.d.born, person.d.death);
        ST_insert(G->st, person);           // li inserisco nell'ST
    }

    // Leggo gli archi 
    char nome1[MAXS], nome2[MAXS];
    Date d1, d2;
    
    while (fscanf(fp, "%s %s %s %s %s %s", nome1, d1.born, d1.death, nome2, d2.born, d2.death) == 6) {
        
        Key parent, child;
        strcpy(parent.nome, nome1);
        parent.d = d1;
        strcpy(child.nome, nome2);
        child.d = d2;
        
        int id_parent = ST_search(G->st, parent);       // cerco gli indici nella ST
        int id_child = ST_search(G->st, child);
        
        if (id_parent != -1 && id_child != -1) {
            
            // inserisco in testa nella lista delle adiacenze 
            G->ladj[id_parent] = new_node(id_child, G->ladj[id_parent]);        // inserisco nella lista normale

            G->ladj_rev[id_child] = new_node(id_parent, G->ladj_rev[id_child]);         // inserisco anche l'arco al contrario nella lista trasposta (da figlio a padre)
        }
    }

    fclose(fp);
    return G;
}


// funzione per la prima parte della ricerca, visita in ampiezza in discesa del grafo, (praticamente non modificata)
bool bfs_check(int id1, int target, link *ladj, int V, int k){        // tra i parametri dimentico k
    Q queue = Q_init(V);
    int *vect_dist = malloc(V*sizeof(int));
    for (int i = 0; i < V; i++){
        vect_dist[i] = -1;
    }

    Q_insert(queue, id1);
    vect_dist[id1] = 0;
    int curr; 
    link t;

    while (!Q_empty(queue)){
        curr = Q_extract(queue);
        if (vect_dist[curr] >= k) continue;      // aggiungo questo controllo così non esploro oltre il grado k

        for (t = ladj[curr]; t != NULL; t = t->next) {  // analizzo la lista di adicenza dell'elemento estratto, in sede d'esame, dalla fretta, ho dimenticato ladj[];
            if(t->w == target && vect_dist[curr] + 1 < k){      // ho trovato il target a distanza <= k
                Q_free(queue); free(vect_dist);
                return true;
            }

            if (vect_dist[t->w] == -1){     // se non ci sono passato estraendo gli altri verici
                Q_insert(queue, t->w);
                vect_dist[t->w] = vect_dist[curr] + 1;      // aggiorno la distanza ed aggiungo in coda
            }
        }       
    }

    Q_free(queue);
    free(vect_dist);
    return false;

}

// BFS sul grafo trasposto per cercare gli antenati comuni, AGGIUNTA (BFS standard)
void bfs_reversed(int id, link *ladj_rev, int V, int *dist_vec, int k) {
    Q queue = Q_init(V);
    dist_vec[id] = 0;
    Q_insert(queue, id);

    while (!Q_empty(queue)) {
        int curr = Q_extract(queue);

        if (dist_vec[curr] >= k) continue;      // mi limito sempre a distanza <= K

        for (link t = ladj_rev[curr]; t != NULL; t = t->next) {

            if (dist_vec[t->w] == -1) {
                dist_vec[t->w] = dist_vec[curr] + 1;
                Q_insert(queue, t->w);
            }
        }
    }

    Q_free(queue);
}

// funzione di verifica richiesta, prima casistica uguale a quella d'esame, la seconda completamente rivisitata
bool relationshipK(GRAPH G, char *name1, Date d1, char *name2, Date d2, int k){      // parametri da prototipo, qua il puntatore alla struct graph è GRAPH
    Key person1, person2;
    strcpy(person1.nome, name1); person1.d = d1;        // creo le due chiavi da poter ricercare nell'albero 
    strcpy(person2.nome, name2); person2.d = d2;
    int id1 = ST_search(G->st, person1);
    int id2 = ST_search(G->st, person2);

    // controllo la validità delle persone, AGGIUNTA
    if(id1 != -1 && id2 != -1){
        printf("\npersone non presenti nel grafo\n");
        return false;
    }

    bool are_k_relation = false;
    
    // controllo prima casistica: uno è discentente dell'altro distanti <= K

    if(bfs_check(id1, id2, G->ladj, G->V, k) || bfs_check(id2, id1, G->ladj, G->V, k)) are_k_relation = true;


    // SECONDO CASO, LAVORO CON IL GRAFO TRASPOSTO, RISCRITTA E RIPENSATA DA ZERO
    else{
        int *dist1 = malloc(G->V*sizeof(int));
        int *dist2 = malloc(G->V*sizeof(int));
        int i;
        for (i = 0; i < G->V; i++){
            dist1[i] = -1;
            dist2[i] = -1;
        }
        // BFS in salita
        bfs_reversed(id1, G->ladj_rev, G->V, dist1, k);
        bfs_reversed(id2, G->ladj_rev, G->V, dist2, k);
    
        for (i = 0; i < G->V && !are_k_relation; i++){
            if (dist1[i] != -1 && dist2[i] != -1){  // hanno un antenato comune
                if(dist1[i] + dist2[i] < k) {           // controllo le distanze 
                    are_k_relation = true;
                }
            }
        }
        free(dist1);
        free(dist2);
    }


    return are_k_relation;

}

// BFS per il problema di ricerca (terza richiesta), sostanzialmente non modificata
void bfs_opt (int id, GRAPH G, int *dist, int k){
    Q queue = Q_init(G->V);
    Q_insert(queue, id);
    dist[id] = 0;
    int curr;
    link t;
    while (!Q_empty(queue)){
        curr = Q_extract(queue);

        // blocco la BFS a distanza K
        if (dist[curr] == k) continue;        // sistemo meglio la condizione di terminazione perchè nel compito avevo gestito erroneamente questo controllo

        for (t = G->ladj[curr]; t != NULL; t = t->next){
            if (dist[t->w] == -1){
                dist[t->w] = dist[curr] +1;     // aggiorno la distanza
                Q_insert(queue, t->w); // inserisco i nuovi veritici nella coda

            }
        }
    }
}

// funzione di ricerca richiesta, praticamente ricopiata, l'unica modifica riguarda l'allocazione del vettore dist (che prima veniva allocata/liberato ad ogni iterazione)
void largestLineageK(GRAPH G, int k){
    int top_person;
    int max_count = 0;
    int i, j;
    int count; 
    int *dist = malloc(G->V*sizeof(int)); // alloco un vettore distanze che parte da -1 per ogni veritce da cui poi faccio una BFS

    for(i = 0; i < G->V; i++){
        count = 0;              // resetto il counter dei veritici raggiunti dalla BFS 

        for (j = 0; j < G->V; j++){     // resetto tutte le distanze perchè sto analizzando un nuovo vertice
            dist[j] = -1;
        }

        bfs_opt(i, G, dist, k);     // faccio la BFS dal vertice corrente

        for (j = 0; j < G->V; j++){
            if (dist[j] != -1) count++;     // conto i discendenti a distanza <= K
        }

        if (count > max_count) {
            max_count = count;
            top_person = i;
        }
        
    }
    free(dist);

    Key person = ST_search_by_index(G->st, top_person);
    printf ("\n-->persona con più discendenti %s", person.nome);

    return;
}


// Free della lista
void free_list(link head) {
    link t, next;
    for (t = head; t != NULL; t = next) {
        next = t->next;
        free(t);
    }
}

// Free del grafo
void graph_free(GRAPH G) {
    if (G == NULL) return;
    
    for (int i = 0; i < G->V; i++) {
        free_list(G->ladj[i]);
        free_list(G->ladj_rev[i]);
    }
    free(G->ladj);
    free(G->ladj_rev);
    ST_free(G->st);
    free(G);
}