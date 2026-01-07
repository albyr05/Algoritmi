#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_N 30
#define MAX_S 31

typedef struct { int u, v, wt; } Edge;

int N, E = 0;
char nomi[MAX_N][MAX_S];
int adj[MAX_N][MAX_N];
Edge archi[MAX_N * MAX_N];
int rimossi[MAX_N * MAX_N];

// Array per memorizzare TUTTI gli insiemi di cardinalità minima
typedef struct {
    int archi_rimossi[MAX_N * MAX_N];
    int peso_totale;
} InsiemeMinimo;

InsiemeMinimo insiemi_minimi[10000]; // Array per memorizzare tutti gli insiemi
int num_insiemi_trovati = 0;
int cardinalita_minima = -1;

// --- 1. VERIFICA DAG (DFS) ---
int check_cycle(int v, int *visited, int *stack) {
    visited[v] = 1; 
    stack[v] = 1;
    for (int w = 0; w < N; w++) {
        if (adj[v][w] != -1) {
            if (!visited[w] && check_cycle(w, visited, stack)) return 1;
            else if (stack[w]) return 1; // Trovato arco all'indietro
        }
    }
    stack[v] = 0; 
    return 0;
}

int is_dag() {
    int visited[MAX_N] = {0}, stack[MAX_N] = {0};
    for (int i = 0; i < N; i++)
        if (!visited[i] && check_cycle(i, visited, stack)) return 0;
    return 1;
}

// --- 2. RICERCA COMBINATORIA (FAS) - CORRETTO ---
void find_fas(int start, int k, int current_k) {
    if (current_k == k) {
        if (is_dag()) {
            // Calcola il peso dell'insieme corrente
            int peso_attuale = 0;
            for (int i = 0; i < E; i++) 
                if (rimossi[i]) peso_attuale += archi[i].wt;
            
            // Memorizza questo insieme
            if (num_insiemi_trovati < 10000) {
                for (int i = 0; i < E; i++) {
                    insiemi_minimi[num_insiemi_trovati].archi_rimossi[i] = rimossi[i];
                }
                insiemi_minimi[num_insiemi_trovati].peso_totale = peso_attuale;
                num_insiemi_trovati++;
            }
        }
        return;
    }
    
    // Esplora combinazioni di archi da rimuovere
    for (int i = start; i <= E - (k - current_k); i++) {
        int u = archi[i].u, v = archi[i].v, w = archi[i].wt;
        adj[u][v] = -1; // Rimuovi temporaneamente
        rimossi[i] = 1;
        find_fas(i + 1, k, current_k + 1);
        adj[u][v] = w;  // Ripristina
        rimossi[i] = 0;
    }
}

// --- 3. DISTANZE MASSIME ---
void topological_sort(int v, int *visited, int *stack, int *pos) {
    visited[v] = 1;
    for (int w = 0; w < N; w++)
        if (adj[v][w] != -1 && !visited[w]) 
            topological_sort(w, visited, stack, pos);
    stack[(*pos)--] = v;
}

void calcola_distanze() {
    int visited[MAX_N] = {0}, stack[MAX_N], pos = N - 1;
    
    // Esegui ordinamento topologico
    for (int i = 0; i < N; i++) 
        if (!visited[i]) 
            topological_sort(i, visited, stack, &pos);
    
    printf("Calcolo distanze massime\n");

    
    // Per ogni possibile nodo sorgente
    for (int s = 0; s < N; s++) {
        int dist[MAX_N];
        for (int i = 0; i < N; i++) dist[i] = INT_MIN; 
        dist[s] = 0;
        
        // Calcola distanze seguendo l'ordine topologico
        for (int i = 0; i < N; i++) {
            int u = stack[i];
            if (dist[u] != INT_MIN) {
                for (int v = 0; v < N; v++) {
                    if (adj[u][v] != -1) {
                        if (dist[u] + adj[u][v] > dist[v])
                            dist[v] = dist[u] + adj[u][v];
                    }
                }
            }
        }
        
        // Stampa risultati
        printf("\nSorgente %s:\n", nomi[s]);
        for (int i = 0; i < N; i++) {
            if (dist[i] == INT_MIN) 
                printf("  -> %s: non raggiungibile\n", nomi[i]);
            else 
                printf("  -> %s: %d\n", nomi[i], dist[i]);
        }
    }
}

// Funzione per stampare un insieme di archi
void stampa_insieme(InsiemeMinimo *ins, int indice) {
    printf("Insieme %d (peso totale: %d):\n", indice + 1, ins->peso_totale);
    printf("  Archi da rimuovere: ");
    int primo = 1;
    for (int i = 0; i < E; i++) {
        if (ins->archi_rimossi[i]) {
            if (!primo) printf(", ");
            printf("%s->%s(%d)", nomi[archi[i].u], nomi[archi[i].v], archi[i].wt);
            primo = 0;
        }
    }
    printf("\n");
}

// Funzione per stampare il DAG
void stampa_dag() {
    printf("\nDAG risultante:\n");
    for (int i = 0; i < N; i++) {
        int ha_archi = 0;
        for (int j = 0; j < N; j++) {
            if (adj[i][j] != -1) ha_archi = 1;
        }
        if (ha_archi) {
            printf("  %s:", nomi[i]);
            for (int j = 0; j < N; j++) {
                if (adj[i][j] != -1) {
                    printf(" -> %s(%d)", nomi[j], adj[i][j]);
                }
            }
            printf("\n");
        }
    }
}

int main() {
    FILE *f = fopen("grafo.txt", "r");
    if (!f) {
        fprintf(stderr, "Errore: impossibile aprire grafo.txt\n");
        return 1;
    }
    
    // Lettura del grafo
    fscanf(f, "%d", &N);
    for (int i = 0; i < N; i++) {
        fscanf(f, "%s", nomi[i]);
        for (int j = 0; j < N; j++) adj[i][j] = -1;
    }
    
    char s1[MAX_S], s2[MAX_S]; 
    int wt;
    while (fscanf(f, "%s %s %d", s1, s2, &wt) == 3) {
        int u = -1, v = -1;
        for (int i = 0; i < N; i++) {
            if (strcmp(nomi[i], s1) == 0) u = i;
            if (strcmp(nomi[i], s2) == 0) v = i;
        }
        if (u != -1 && v != -1) {
            adj[u][v] = wt;
            archi[E].u = u; 
            archi[E].v = v; 
            archi[E].wt = wt;
            E++;
        }
    }
    
    
    printf("Grafo originale caricato:\n");
    printf("  Nodi: %d\n", N);
    printf("  Archi: %d\n\n", E);
    for (int i = 0; i < N; i++) {
        int ha_archi = 0;
        for (int j = 0; j < N; j++) {
            if (adj[i][j] != -1) ha_archi = 1;
        }
        if (ha_archi) {
            printf("  %s:", nomi[i]);
            for (int j = 0; j < N; j++) {
                if (adj[i][j] != -1) {
                    printf(" -> %s(%d)", nomi[j], adj[i][j]);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
    
    // OPERAZIONE 1: Trova TUTTI gli insiemi di cardinalità minima
    printf("Individuazione insiemi minimi\n");
    
    
    if (is_dag()) {
        printf("Il grafo è già un DAG, non serve rimuovere archi.\n");
    } else {
        printf("Ricerca di TUTTI gli insiemi di cardinalità minima...\n\n");
        
        // Cerca per cardinalità crescente finché non trovi insiemi
        for (int k = 1; k <= E; k++) {
            int insiemi_prima = num_insiemi_trovati;
            find_fas(0, k, 0);
            
            if (num_insiemi_trovati > insiemi_prima) {
                // Trovata la cardinalità minima
                cardinalita_minima = k;
                printf("Cardinalità minima trovata: %d archi\n", cardinalita_minima);
                printf("Numero totale di insiemi: %d\n\n", num_insiemi_trovati);
                break;
            }
        }
        
        // Mostra tutti gli insiemi trovati
        if (num_insiemi_trovati > 0) {
            printf("Elenco di tutti gli insiemi:\n");
            printf("----------------------------\n");
            for (int i = 0; i < num_insiemi_trovati; i++) {
                stampa_insieme(&insiemi_minimi[i], i);
            }
        }
        
        // OPERAZIONE 2: Scegli l'insieme a peso massimo
        printf("\n");
        printf("Costruzione DAG peso massimo\n");
        
        
        int idx_max_peso = 0;
        int max_peso = insiemi_minimi[0].peso_totale;
        
        for (int i = 1; i < num_insiemi_trovati; i++) {
            if (insiemi_minimi[i].peso_totale > max_peso) {
                max_peso = insiemi_minimi[i].peso_totale;
                idx_max_peso = i;
            }
        }
        
        printf("Insieme scelto (peso massimo = %d):\n", max_peso);
        stampa_insieme(&insiemi_minimi[idx_max_peso], idx_max_peso);
        
        // Applica la rimozione definitiva
        for (int i = 0; i < E; i++) {
            if (insiemi_minimi[idx_max_peso].archi_rimossi[i]) {
                adj[archi[i].u][archi[i].v] = -1;
            }
        }
        
        stampa_dag();
    }
    
    // OPERAZIONE 3: Calcola distanze massime
    calcola_distanze();
    
    fclose(f);
    
    return 0;
}