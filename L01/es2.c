#include <stdio.h>
#include <string.h>

typedef struct {
    char code[30];
    char partenza[30];
    char destinazione[30];
    int anno, mese, giorno, ora_p, min_p, sec_p, ora_a, min_a, sec_a, ritardo;
} tratta;

typedef enum{r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine, r_exit} comando_e;

int leggi_comando();
int leggi_file(FILE *fp, tratta v[]);
void menu_parola(comando_e input, tratta v[], int nr);
void check_date(tratta v[], int nr);
void stazione(char more_info[], tratta v[], int nr, int choose);
void check_late(tratta v[], int nr);
void check_total_late(tratta v[], int nr);
void print_tratta (tratta log);
int comprese(tratta log,  int anno_i, int mese_i, int giorno_i, int anno_f, int  mese_f, int giorno_f );


int main(void) {
    FILE *fp = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab01/es2input.txt", "r");
    if (fp == NULL) {
        printf("errore nell'apertura del file");
        return -1;
    }

    comando_e input = r_date;
    tratta v[1000];
    int nr = leggi_file(fp, v);

    while (input != r_fine && input != r_exit) {
        input = leggi_comando();
        menu_parola(input, v, nr);
        if (input == r_exit) {
            return r_exit;
        }
    }

    return 0;
}



int leggi_file(FILE *fp, tratta v[]) {
    int nr, i ;
    fscanf (fp, "%d", &nr);
    for (i = 0; i < nr; i++) {
        fscanf (fp, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", v[i].code, v[i].partenza, v[i].destinazione, &v[i].anno, &v[i].mese, &v[i].giorno, &v[i].ora_p, &v[i].min_p,
            &v[i].sec_p, &v[i].ora_a, &v[i].min_a, &v[i].sec_a, &v[i].ritardo);
    }
    return nr;
}


int leggi_comando () {
    comando_e comando = 0;
    char cmd[31], words[r_exit][31] = {"date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine"};
    int trovato = 0;
    //ACQUISIZIONE COMANDO DA EFFETTUARE
    printf("\nInserire un comando tra \"date\", \"partenza\", \"capolinea\", \"ritardo\", \"ritardo_tot\" e \"fine\": ");
    scanf("%s", cmd); //acquisisco il comando
    while (comando < r_exit && !trovato) //controllo che il comando sia presente nella lista di parole WORDS
    {
        if (!strcmp(words[comando], cmd))
            trovato = 1;
        else
            comando ++;
    }
    return comando; //restituisco direttamente il numero del comando corrispondente
}


void menu_parola(comando_e input, tratta v[], int nr) {
    char more_info[31];
    switch (input) {
        case r_date: {
            check_date(v, nr);
            break;
        }
        case r_partenza: {
            printf("Inserisci la stazione di partenza: "); scanf("%s", more_info);
            stazione(more_info, v, nr, 0);
            break;
        }
        case r_capolinea: {
            printf("Inserisci la stazione di arrivo: "); scanf("%s", more_info);
            stazione(more_info, v, nr, 1);
            break;
        }
        case r_ritardo: {
            check_late(v, nr);
            break;
        }
        case r_ritardo_tot: {
            check_total_late(v, nr);
            break;
        }
        case r_fine: {
            printf("Arrivederci"); break;
        }
        case r_exit: {
            printf("errore nell'acquisizione dell'input");
            break;
        }
    }
}


void print_tratta(tratta log) {
    printf("\n%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", log.code, log.partenza, log.destinazione, log.anno, log.mese, log.giorno, log.ora_p, log.min_p, log.sec_p, log.ora_a, log.min_a, log.sec_a, log.ritardo);
}


int comprese(tratta log,  int anno_i, int mese_i, int giorno_i, int anno_f, int  mese_f, int giorno_f ) {
    char inizio[9], fine[9], attuale[9];
    sprintf(inizio, "%04d%02d%02d", anno_i, mese_i, giorno_i);
    sprintf(fine, "%04d%02d%02d", anno_f, mese_f, giorno_f);
    sprintf(attuale, "%04d%02d%02d", log.anno, log.mese, log.giorno);
    if (strcmp(attuale, inizio) > 0 && strcmp(attuale, fine) < 0 ) {
        return 1;
    }
    return 0;
}


void check_date(tratta v[], int nr) {
    int anno_i, mese_i, giorno_i, anno_f, mese_f, giorno_f;
    printf("Inserisci l'intervallo di date nel formato (yyyy/mm/gg)");
    scanf ("%d/%d/%d %d/%d/%d", &anno_i, &mese_i, &giorno_i, &anno_f, &mese_f, &giorno_f);
    int i, found = 0 ;
    for (i = 0; i < nr; i++) {
        if (comprese(v[i], anno_i, mese_i, giorno_i, anno_f, mese_f, giorno_f) == 1 ) {
            found = 1;
            print_tratta(v[i]);
        }
    }
    if (found == 0) {
        printf("non ho trovato nessuna tratta nel range indicato");
    }
}


void stazione(char more_info[], tratta v[], int nr, int choose) {
    int i, found = 0;
    for (i = 0; i < nr; i++) {
        if (choose == 0) {
            if (strcmp(v[i].partenza, more_info) == 0) {
                print_tratta(v[i]);
                found = 1;
            }
        }
        else if (choose == 1) {
            if (strcmp(v[i].destinazione, more_info) == 0) {
                print_tratta(v[i]);
                found = 1;
            }
        }
    }
    if (found == 0) {
        if (choose == 0) {
            printf ("Non ho trovato nessuna tratta con questa partenza (%s)", more_info);
        }
        else if (choose == 1 ) {
            printf ("Non ho trovato nessuna tratta con questa destinazione (%s)", more_info);

        }
    }
}


void check_late(tratta v[], int nr) {
    int anno_i, mese_i, giorno_i, anno_f, mese_f, giorno_f;
    printf("Inserisci l'intervallo di date nel formato (yyyy/mm/gg)");
    scanf ("%d/%d/%d %d/%d/%d", &anno_i, &mese_i, &giorno_i, &anno_f, &mese_f, &giorno_f);
    int i, found = 0 ;
    for (i = 0; i < nr; i++) {
        if (comprese(v[i], anno_i, mese_i, giorno_i, anno_f, mese_f, giorno_f) == 1 && v[i].ritardo != 0) {
            print_tratta(v[i]);
            found = 1;
        }
    }
    if (found == 0) {
        printf("Nessuna corsa ha raggiunto la destinazione in ritardo nel range di dete inserito");
    }
}


void check_total_late(tratta v[], int nr) {
    int i,  total = 0;
    char target_code[31];
    printf("Inserisci un codice di tratta di cui vuoi conoscere il ritardo complessivo (GTTxxx): ");
    scanf("%s", target_code);
    for (i = 0; i < nr; i++) {
        if (strcmp(v[i].code, target_code) == 0) {
            total += v[i].ritardo;
        }
    }
    if (total > 0) {
        printf ("La tratta %s ha accumulato un ritardo totale di %d minuti", target_code, total);
    }
    else {
        printf("la tratta %s non ha accumulato nessun minuto di ritardo", target_code);
    }

}

