#include <stdio.h>
#include <string.h>

typedef struct {
    char code[30];
    char departing[30];
    char destination[30];
    char data[30];
    char ora_p[30];
    char ora_a[30];
    int late;
} tratta;

int leggi_file(FILE *fp, tratta v[]);
int compare_struct (tratta *a, tratta *b, int par);
void sort_struct (tratta *ordina_date[], int nr, int par);
void print_struct (int nr, tratta *v[]);

int main(void) {
    FILE *fp = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab01/es2input.txt", "r");
    if (fp == NULL) {
        printf("errore nell'apertura del file");
        return -1;
    }
    tratta v[100];                  // preparo il vettore di struct riempito con le righe del file
    int nr = leggi_file(fp, v);

    tratta *ordina_date[nr], *ordina_departing[nr], *ordina_code[nr], *ordina_destination[nr]; 
    for (int i = 0; i < nr; i++){
        ordina_code[i] = &v[i];
        ordina_date[i] = &v[i];
        ordina_departing[i] = &v[i];
        ordina_destination[i] = &v[i];
    }
    sort_struct(ordina_date, nr, 2);            // ordina data
    sort_struct(ordina_code, nr, 3);            // ordina codice
    sort_struct(ordina_departing, nr, 4);       // ordina partenza
    sort_struct(ordina_destination, nr, 5);     // ordina arrivo

    printf("ORDINAMENTO CODICE\n");
    print_struct(nr, ordina_code);
    printf("\n\nORDINAMENTO DATA\n");
    print_struct(nr, ordina_date);
    printf("\n\nORDINAMENTO PARTENZA\n");
    print_struct(nr, ordina_departing);
    printf("\n\nORDINAMENTO ARRIVO\n");
    print_struct(nr, ordina_destination);  

    return 0;
}

int leggi_file(FILE *fp, tratta v[]) {
    int nr, i ;
    fscanf (fp, "%d", &nr);
    for (i = 0; i < nr; i++) {
        fscanf (fp, "%s %s %s %s %s %s %d", v[i].code, v[i].departing, v[i].destination, v[i].data, v[i].ora_p, v[i].ora_a,  &v[i].late);
    }
    return nr;
}

int compare_struct(tratta *a, tratta *b, int par){
    if (par == 2) {
        if (strcmp(a->data, b->data) == 0){
            return strcmp(a->ora_p, b->ora_p);
        }
        return strcmp(a->data, b->data);
    }
    else if (par == 3) return strcmp(a->code, b->code);
    else if (par == 4) return strcmp (a->departing, b->departing);
    else if (par == 5) return strcmp (a->destination, b->destination);
    return 0;
}

void sort_struct(tratta *vp[], int nr, int par) {
    int i, j, flag = 1;
    tratta *temp;
    for (i = 0; i < nr && flag == 1; i++) {
        flag = 0;
        for (j = 0; j < nr - i - 1; j++) {
            if (compare_struct(vp[j], vp[j+1], par) > 0) {
                temp = vp[j];
                vp[j] = vp[j+1];
                vp[j+1] = temp;
                flag = 1;
            }
        }
    }
}

void print_struct(int nr, tratta *v[]){
    for (int i = 0; i < nr; i++){
        printf("%s %s %s %s %s %s %d\n", v[i]->code, v[i]->departing, v[i]->destination, v[i]->data, v[i]->ora_p, v[i]->ora_a, v[i]->late);
    }
}




