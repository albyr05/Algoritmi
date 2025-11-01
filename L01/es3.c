#include <stdio.h>
#include <string.h>

// Funzione per svuotare il buffer di input fino al newline o EOF
void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Definizione della struttura tratta che rappresenta una tratta ferroviaria
typedef struct {
    char code[30];
    char partenza[30];
    char destinazione[30];
    char data[30];
    char ora_p[30];
    char ora_a[30];
    int ritardo;
} tratta;           // definisco la struttura base per ogni elemento

// Enumerazioni per i comandi e tipi di ricerca
typedef enum{stampa, stampa_su_file, ordina_date, ordina_tratta, ordina_partenza, ordina_arrivo, ricerca, fine, error} comando_e;
typedef enum{ricerca_stazione, ricerca_codice}ricerca_e;

//---------------------------------------------------------------------------
// Funzione che legge dal file le informazioni delle tratte e le memorizza nel vettore v
int leggi_file(FILE *fp, tratta v[]);
// Funzione che stampa le informazioni delle tratte su un file o stdout
void print_struct (tratta v[], int nr, FILE *fpout);
// Funzione che gestisce il menu in base al comando inserito dall'utente
void menu_parola(comando_e input, tratta v[], int nr);
// Funzione che legge il comando inserito dall'utente e lo converte in enum
int leggi_comando ();
// Funzione che ordina il vettore di tratte in base al parametro passato
void sort_struct (tratta v[], int nr, comando_e par);
// Funzione di confronto tra due tratte in base al parametro passato
int compare_par(tratta a, tratta b, comando_e par );
// Funzione che ricerca le tratte in base alla stazione di partenza o codice
void search_departing(tratta v[], int nr, char station[], ricerca_e par);
// Funzione di ricerca dicotomica (binaria) per trovare le tratte corrispondenti
void dicotomic_reaserch (tratta *v, int l, int r, char *station, ricerca_e par);
// Funzione di controllo per confrontare una tratta con la stringa di ricerca
int check(tratta a, char *cmpto, ricerca_e par);

//---------------------------------------------------------------------------

int main(void) {
    // Apro il file di input in lettura
    FILE *fp = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab01/es2input.txt", "r");
    if (fp == NULL) {
        printf("errore nell'apertura del file\n");
        return -1;
    }

    comando_e input;
    tratta v[100];
    // Leggo le tratte dal file e ottengo il numero di elementi letti
    int nr = leggi_file(fp, v);
    fclose(fp);

    flush_input();
    do {
        // Leggo il comando inserito dall'utente
        input = leggi_comando();
        if (input == error) {
            printf("INPUT ERRATO\n");
            continue;  // Richiedi il comando senza eseguire menu_parola
        }
        // Eseguo l'azione corrispondente al comando
        menu_parola(input, v, nr);
    } while (input != fine);

    printf("\nArrivederci\n");
    return 0;
}

// Funzione che legge dal file il numero di tratte e i dati di ciascuna tratta
int leggi_file(FILE *fp, tratta v[]) {
    int nr, i ;
    fscanf (fp, "%d", &nr);  // leggo il numero di tratte
    for (i = 0; i < nr; i++) {
        // leggo i campi della tratta uno per uno
        fscanf (fp, "%29s %29s %29s %29s %29s %29s %d", v[i].code, v[i].partenza, v[i].destinazione, v[i].data, v[i].ora_p, v[i].ora_a,  &v[i].ritardo);
    }
    return nr;
}

// Funzione che legge il comando inserito dall'utente e lo converte nel corrispondente enum
int leggi_comando () {
    comando_e comando = 0;
    // Array di stringhe contenente i comandi validi
    char cmd[31], words[8][31] = {"stampa_out", "stampa_su_file", "ordina_date", "ordina_tratta", "ordina_partenza", "ordina_arrivo", "ricerca", "fine"};
    int trovato = 0;
    
    printf("\n\nInserire un comando tra \"stampa_out\", \"stampa_su_file\", \"ordina_date\", \"ordina_tratta\", \"ordina_partenza\", \"ordina_arrivo\", \"ricerca\" e \"fine\":");
    
    if (scanf("%30s", cmd) != 1) {
        flush_input();
        return error;
    }
    flush_input();
    
    // Cerco il comando inserito all'interno dell'array words
    while (comando < error && !trovato) {
        if (!strcmp(words[comando], cmd))
            trovato = 1;
        else
            comando ++;
    }
    return comando;
}

// Funzione che gestisce il menu e le azioni in base al comando inserito
void menu_parola(comando_e input, tratta v[], int nr) {
    switch (input) {
        case stampa:{
            // Stampo tutte le tratte su stdout
            FILE *fp_out = stdout;
            print_struct(v, nr, fp_out);
            break;
        }
        case stampa_su_file:{
            // Apro il file di output e stampo tutte le tratte su file
            FILE *fpout = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab01/es3output.txt", "w");
            print_struct(v, nr, fpout);
            fclose(fpout);  
            break;
        }
        case ordina_date:
        case ordina_tratta:
        case ordina_partenza:
        case ordina_arrivo:{
            // Ordino le tratte in base al parametro e stampo a video
            sort_struct(v, nr, input);
            print_struct(v, nr, stdout);
            break;
        }
        case ricerca: {
            ricerca_e par;
            // Chiedo se la ricerca deve essere per stazione o codice
            printf("vuoi ricercare una stazione di partenza o un codice [0/1]: ");
            scanf("%d", &par);
            flush_input();
            char station[30];

            switch (par){
                case ricerca_stazione:{
                    // Richiedo la stazione da ricercare
                    printf("Inserisci la stazione di ricerca: ");
                    scanf("%29s", station);
                    flush_input();
                    // Ordino per stazione di partenza per ricerca binaria
                    sort_struct(v, nr, ordina_partenza);
                    // Eseguo ricerca dicotomica
                    dicotomic_reaserch(v, 0, nr-1, station, par);
                    break;
                }
                case ricerca_codice:{
                    // Richiedo il codice tratta da ricercare
                    printf("Inserisci il codice tratta: ");
                    scanf("%29s", station);
                    flush_input();
                    // Ordino per codice tratta per ricerca binaria
                    sort_struct(v, nr, ordina_tratta);
                    // Eseguo ricerca dicotomica
                    dicotomic_reaserch(v, 0, nr-1, station, par);
                    break;
                }
            }
            break;
        }
        case fine: {
            // Comando di uscita, non fa nulla
            break;
        }    
        case error: {
            // Comando errato, non fa nulla
            break;
        }
    }

}

// Funzione che stampa le informazioni delle tratte su file o stdout
void print_struct(tratta v[], int nr, FILE *fpout){
    int i;
    for (i = 0; i < nr; i++){
        fprintf(fpout, "\n%s %s %s %s %s %s %d", v[i].code, v[i].partenza, v[i].destinazione, v[i].data, v[i].ora_p, v[i].ora_a,  v[i].ritardo);
    }
}

// Funzione che confronta due tratte in base al parametro passato per l'ordinamento
int compare_par (tratta a, tratta b, comando_e par){
    if (par == ordina_date) {
        // Se la data è uguale confronto l'ora di partenza
        if (strcmp(a.data, b.data) == 0){
            return strcmp(a.ora_p, b.ora_p);
        }
        // Altrimenti confronto la data
        return strcmp(a.data, b.data);
    }
    else if (par == ordina_tratta) return strcmp(a.code, b.code);
    else if (par == ordina_partenza) return strcmp (a.partenza, b.partenza);
    else if (par == ordina_arrivo) return strcmp (a.destinazione, b.destinazione);
    return 0;
}

// Funzione che ordina il vettore di tratte con algoritmo bubble sort in base al parametro passato
void sort_struct(tratta v[], int nr, comando_e par){

    int i, j, flag = 1;
    tratta temp; 
    for (i = 0; i < nr && flag == 1; i++){
        flag = 0;  // Flag per verificare se ci sono stati scambi nell'iterazione
        for (j = 0; j < nr-i-1; j ++){
            // Confronto gli elementi adiacenti e scambio se necessario
            if (compare_par(v[j], v[j+1], par) > 0){
                flag = 1;
                temp = v[j+1]; v[j+1] = v[j]; v[j] = temp;
            }
        }
    }
}

// Funzione che ricerca linearmente le tratte in base alla stazione di partenza o codice
void search_departing(tratta v[], int nr, char *station, ricerca_e par){
    int i = 0, flag = 0;
    
    for (i = 0; i < nr; i++){
        if (par == ricerca_stazione){
            // Controllo se la stazione di partenza inizia con la stringa station
            if (strstr(v[i].partenza, station) == v[i].partenza){           
                // Stampo la tratta trovata
                printf("\n%s %s %s %s %s %s %d", v[i].code, v[i].partenza, v[i].destinazione, v[i].data, v[i].ora_p, v[i].ora_a,  v[i].ritardo);
                flag = 1;
            }
        }
        if (par == ricerca_codice){
            // Controllo se il codice tratta corrisponde esattamente
            if (strcmp(v[i].code, station) == 0){         
                printf("\n%s %s %s %s %s %s %d", v[i].code, v[i].partenza, v[i].destinazione, v[i].data, v[i].ora_p, v[i].ora_a,  v[i].ritardo);
                flag = 1;
                break;  // codice unico, esco dal ciclo
            }
        }
    }
    if (flag == 0){
        // Nessuna tratta trovata con la stringa di ricerca
        printf("Nessuna stazione di partenza trovata con %s", station);
    }

}

// Funzione di controllo per confrontare una tratta con la stringa di ricerca in base al tipo di ricerca
int check(tratta a, char *cmpto, ricerca_e par) {
    if (par == ricerca_stazione) {
        // Confronto i primi caratteri della stazione di partenza con la stringa cmpto
        return strncmp(a.partenza, cmpto, strlen(cmpto));
    } else if (par == ricerca_codice) {
        // Confronto esatto del codice tratta
        return strcmp(a.code, cmpto);
    }
    return -1;
}

// Funzione di ricerca binaria che cerca la stringa station nel vettore ordinato v
void dicotomic_reaserch(tratta *v, int l, int r, char *station, ricerca_e par) {
    if (l > r){
        // Caso base: intervallo vuoto, nessuna tratta trovata
        printf("nessuna tratta trovata");
        return;
    }

    int m = (l + r) / 2;
    int result = check(v[m], station, par);

    if (result == 0) {
        // Ho trovato una corrispondenza esatta, stampo la tratta trovata
        printf("\n%s %s %s %s %s %s %d",
               v[m].code, v[m].partenza, v[m].destinazione,
               v[m].data, v[m].ora_p, v[m].ora_a, v[m].ritardo);

        // Cerco eventuali altre corrispondenze vicine a m (a sinistra e destra)
        int i = m - 1, j = m + 1;
        if (par == ricerca_stazione){
            // Per stazione di partenza, possono esserci più risultati con stessa prefisso
            while (i >= l && check(v[i], station, par) == 0) {
                printf("\n%s %s %s %s %s %s %d",
                    v[i].code, v[i].partenza, v[i].destinazione,
                    v[i].data, v[i].ora_p, v[i].ora_a, v[i].ritardo);
                i--;
            }
            while (j <= r && check(v[j], station, par) == 0) {
                printf("\n%s %s %s %s %s %s %d",
                    v[j].code, v[j].partenza, v[j].destinazione,
                    v[j].data, v[j].ora_p, v[j].ora_a, v[j].ritardo);
                j++;
            }
        }
        return;
    }

    if (result < 0)
        // Se il valore cercato è maggiore del valore a m, cerco nella metà destra
        dicotomic_reaserch(v, m + 1, r, station, par);
    else
        // Altrimenti cerco nella metà sinistra
        dicotomic_reaserch(v, l, m - 1, station, par);

}