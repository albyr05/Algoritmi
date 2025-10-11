#include <stdio.h>
#include <string.h>

typedef struct {
    char code[30];
    char partenza[30];
    char destinazione[30];
    char data[30];
    char ora_p[30];
    char ora_a[30];
    int ritardo;
} tratta;           // difinisco la struttura base per ogni elemento

// gestisco tutte le richieste dell'utente
typedef enum{stampa, stampa_su_file, ordina_date, ordina_tratta, ordina_partenza, ordina_arrivo, ricerca, fine, error} comando_e;

//---------------------------------------------------------------------------
int leggi_file(FILE *fp, tratta v[]);
void print_struct (tratta v[], int nr, FILE *fpout);
void menu_parola(comando_e input, tratta v[], int nr);
int leggi_comando ();
void sort_struct (tratta v[], int nr, int par);
int compare_par(tratta a, tratta b, int par );
void search_departing(tratta v[], int nr, char station[]);
void dicotomic_reaserch (tratta *v, int l, int r, char *station);
//---------------------------------------------------------------------------

int main(void) {
    FILE *fp = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab01/es2input.txt", "r");
    if (fp == NULL) {
        printf("errore nell'apertura del file");
        return -1;
    }

    comando_e input = stampa;
    tratta v[100];                  // preparo il vettore di struct riempito con le righe del file
    int nr = leggi_file(fp, v);

    while (input != fine) {                 // leggo le richieste fino a qunado l'utente non digita fine
        input = leggi_comando();
        menu_parola(input, v, nr);
        if (input == fine) {
            return fine;
        }
    }

    return 0;
}

// leggo il file di input
int leggi_file(FILE *fp, tratta v[]) {
    int nr, i ;
    fscanf (fp, "%d", &nr);
    for (i = 0; i < nr; i++) {
        fscanf (fp, "%s %s %s %s %s %s %d", v[i].code, v[i].partenza, v[i].destinazione, v[i].data, v[i].ora_p, v[i].ora_a,  &v[i].ritardo);
    }
    return nr;
}

// per ogni comando inserito restituisco il numero corrispoindente dell'enum
int leggi_comando () {
    comando_e comando = 0;
    char cmd[31], words[8][31] = {"stampa_out", "stampa_su_file", "ordina_date", "ordina_tratta", "ordina_partenza", "ordina_arrivo", "ricerca", "fine"};
    int trovato = 0;
    //ACQUISIZIONE COMANDO DA EFFETTUARE
    printf("\n\nInserire un comando tra \"stampa_out\", \"stampa_su_file\", \"ordina_date\", \"ordina_tratta\", \"ordina_partenza\", \"ordina_arrivo\", \"ricerca\" e \"fine\": ");
    scanf("%s", cmd); //acquisisco il comando
    while (comando < error && !trovato) //controllo che il comando sia presente nella lista di parole WORDS
    {
        if (!strcmp(words[comando], cmd))
            trovato = 1;
        else
            comando ++;
    }
    return comando; //restituisco direttamente il numero del comando corrispondente
}

void menu_parola(comando_e input, tratta v[], int nr) {
    switch (input) {
        case stampa:{               // stampa a video 
            FILE *fp_out = stdout;
            print_struct(v, nr, fp_out);
            break;
        }
        case stampa_su_file:{           // stampa su un file di testo
            FILE *fpout = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab01/es3output.txt", "w");
            print_struct(v, nr, fpout);
            break;
        }
        case ordina_date:               // gestisco tutti gli ordinamenti
        case ordina_tratta:
        case ordina_partenza:
        case ordina_arrivo:{
            sort_struct(v, nr, input);          // passo direttamente come parametro il numero dell'enum associato al comando
            print_struct(v, nr, stdout);
            break;
        }
        case ricerca: {                 // ricerca per stazione
            printf ("Indica la stazione di partenza che vuoi ricercare: ");
            char station[30];
            scanf("%s", station);
            //search_departing(v, nr, station);
            dicotomic_reaserch(v, 0, nr, station);
            break;
        }
        case fine: {                    // uscita dal programma
            printf("\narrivederci");
            break;
        }
        case error: {                   // errore nell'input inserito
            printf("INPUT ERRATO");
            break;
        }
    }
}

// stampo la struttura
void print_struct(tratta v[], int nr, FILE *fpout) {
    int i;
    for (i = 0; i < nr; i++){
        fprintf(fpout, "\n%s %s %s %s %s %s %d", v[i].code, v[i].partenza, v[i].destinazione, v[i].data, v[i].ora_p, v[i].ora_a,  v[i].ritardo);
    }
}

// in base al parametro passato confronto campi diversi delle due strutture
int compare_par (tratta a, tratta b, int par){
    if (par == 2) {
        if (strcmp(a.data, b.data) == 0){
            return strcmp(a.ora_p, b.ora_p);
        }
        return strcmp(a.data, b.data);
    }
    else if (par == 3) return strcmp(a.code, b.code);
    else if (par == 4) return strcmp (a.partenza, b.partenza);
    else if (par == 5) return strcmp (a.destinazione, b.destinazione);
    return 0;
}

// bubble sort per ordinare il vettore in base al parametro passato
void sort_struct(tratta v[], int nr, int par){

    int i, j, flag = 1;
    tratta temp; 
    for (i = 0; i < nr && flag == 1; i++){
        flag = 0;
        for (j = 0; j < nr-i-1; j ++){
            if (compare_par(v[j], v[j+1], par) > 0){
                flag = 1;
                temp = v[j+1]; v[j+1] = v[j]; v[j] = temp;
            }
        }
    }
}

// ricerca la stazione di partenza
void search_departing(tratta v[], int nr, char *station){
    int i = 0, flag = 0;
    
    for (i = 0; i < nr; i++){
        if (strstr(v[i].partenza, station) == v[i].partenza){           // confronto che il puntatore restituito dalla strstr sia al primo carattere della stazione di partenza e la printo
            printf("\n%s %s %s %s %s %s %d", v[i].code, v[i].partenza, v[i].destinazione, v[i].data, v[i].ora_p, v[i].ora_a,  v[i].ritardo);
            flag = 1;
        }
    }
    if (flag == 0){
        printf("Nessuna stazione di partenza trovata con %s", station);
    }

}

// ricerca dicotomica
void dicotomic_reaserch (tratta *v, int l, int r, char *station){
    if (l >= r){
        if (strstr(v[l].partenza, station) == v[l].partenza){
            printf("\n%s %s %s %s %s %s %d", v[l].code, v[l].partenza, v[l].destinazione, v[l].data, v[l].ora_p, v[l].ora_a,  v[l].ritardo);
        }
        return;
    }
    int m = (l+r)/2;
    dicotomic_reaserch(v, l, m, station);
    dicotomic_reaserch(v, m+1, r, station);
}