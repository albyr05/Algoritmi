#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char code[30];
    char partenza[30];
    char destinazione[30];
    char data[30];
    char ora_p[30];
    char ora_a[30];
    int ritardo;
} tratta;


//----------------------------------------------------------------------------------------------------------------------------------------
int alloc_vet(FILE *fp, tratta **v);          
void ask_input (tratta *v, int *flag, tratta **v_data, tratta **v_code, tratta **v_partenza, tratta **v_arrivo, int nr);
void freeall(tratta **v, tratta ***v_data, tratta ***v_code, tratta ***v_partenza, tratta ***v_arrivo);
void print_struct (tratta **v, int nr);
void alloc_dinamic_struct(tratta *v, tratta ***v_data, tratta ***v_code, tratta ***v_partenza, tratta ***v_arrivo, int nr);
void merge_sort_pointers(tratta **tipo, int comparison, int l, int r, int nr);
void merge (tratta **tipo, int comparison, int l, int r, int m, int nr);
int compare_struct(tratta *a, tratta *b, int par);

//----------------------------------------------------------------------------------------------------------------------------------------

int main(){
    FILE *fp = fopen("provaes3.txt", "r");            // reading default file
    if (fp == NULL) {return 1;}
    int flag = 0;
    tratta *vp;
    int nr = alloc_vet(fp, &vp);

    tratta **v_data, **v_code, **v_partenza, **v_arrivo;                
    alloc_dinamic_struct(vp, &v_data, &v_code, &v_partenza, &v_arrivo, nr);
    fclose(fp);
    
    while (flag == 0){
        ask_input(vp, &flag, v_data, v_code, v_partenza, v_arrivo, nr);             // asking user's input
    }
    freeall(&vp, &v_data, &v_code, &v_partenza, &v_arrivo);             // before terminating the process, frees all heap-memory allocated 
    
    return 0;
}

 
void ask_input(tratta *v, int *flag, tratta **v_data, tratta **v_code, tratta **v_partenza, tratta **v_arrivo, int nr){
    int input; int c;
    char filename[50], path[100];

    printf ("Scegli se vuoi ordinare il vettore, leggere un nuovo input o uscire (0/1/2): ");       
    scanf ("%d", &c);
    

    switch (c){
        case 0:             // if it is requested to order the v pointers
            printf("Scegli la chiave di ordinamento (codice[0], partenza[1], destinazione[2], data[3]): ");
            scanf("%d", &input);
                switch (input){
                    case 0:          
                        merge_sort_pointers(v_code, input, 0, nr-1, nr);            // ordina e stampa i vettori per codice
                        print_struct(v_code, nr);
                        break;
                    case 1:
                        merge_sort_pointers(v_partenza, input, 0, nr-1, nr);        // ordina e stampa i vettori per stazione di partenza
                        print_struct(v_partenza, nr);
                        break;
                    case 2:
                        merge_sort_pointers(v_arrivo, input, 0, nr-1, nr);      // ordina e stampa i vettori per stazione di arrivo 
                        print_struct(v_arrivo, nr);
                        break;
                    case 3:
                        merge_sort_pointers(v_data, input, 0, nr-1, nr);        // ordina e stampa i vettori per data 
                        print_struct(v_data, nr);
                        break;
                }
            break;
        
        case 1:
            printf("Inserisci il nome del file in input: ");            // new input file requested
            scanf("%s", filename);
            sprintf(path, "lab02/%s", filename);                // building the new path in a string 
            FILE *newf = fopen(path, "r");      
            if (newf == NULL) {
                printf("Errore: file non trovato\n");
                return;
            }

            freeall(&v, &v_data, &v_code, &v_partenza, &v_arrivo);          // releasing all memory allocated previously
            
            nr = alloc_vet(newf, &v);           // reallocating the new structs 
            alloc_dinamic_struct(v, &v_data, &v_code, &v_partenza, &v_arrivo, nr);          // and vector pointers 
            fclose(newf);
            
            break;

        case 2:         // terminating case 
            *flag = 1;
    }
}

int alloc_vet(FILE *fp, tratta **v){
    int nr;
    fscanf(fp, "%d", &nr);
    tratta *v_help;             // allocating a dinamic struct vector
    v_help = malloc (nr*sizeof(tratta));        
    for (int i = 0; i < nr; i++){
        fscanf (fp, "%s %s %s %s %s %s %d", v_help[i].code, v_help[i].partenza, v_help[i].destinazione, v_help[i].data, v_help[i].ora_p, v_help[i].ora_a, &v_help[i].ritardo);
    }
    *v = v_help;            // making the heap-memory adress visible to main 
    return nr;          // number of lines
}

// prints the each struct after sorting 
void print_struct(tratta **v, int nr){
    for (int i = 0; i < nr; i++){
        printf("%s %s %s %s %s %s %d\n", v[i]->code, v[i]->partenza, v[i]->destinazione, v[i]->data, v[i]->ora_p, v[i]->ora_a, v[i]->ritardo);
    }
}


void alloc_dinamic_struct(tratta *v, tratta ***v_data, tratta ***v_code, tratta ***v_partenza, tratta ***v_arrivo, int nr){
    tratta **v_data_help = malloc(nr*(sizeof(tratta*)));            // allocating 4 vector pointers to struct 
    tratta **v_code_help = malloc(nr*(sizeof(tratta*)));
    tratta **v_partenza_help = malloc(nr*(sizeof(tratta*)));
    tratta **v_arrivo_help = malloc(nr*(sizeof(tratta*)));

    for (int i = 0; i < nr; i ++){
        v_data_help[i] = &v[i];         // associating the adress to the corresponding struct 
        v_code_help[i] = &v[i];
        v_partenza_help[i] = &v[i];
        v_arrivo_help[i] = &v[i];
    }

    *v_data = v_data_help;          // making the allocation visible to main
    *v_code = v_code_help;
    *v_partenza = v_partenza_help;
    *v_arrivo = v_arrivo_help;

}

// merge linearitmic stable sorting 
void merge_sort_pointers(tratta **tipo, int comparison, int l, int r, int nr){
    if (l>=r){
        return;
    }
    int m = (l+r) / 2;
    merge_sort_pointers(tipo, comparison, l, m, nr);
    merge_sort_pointers(tipo, comparison, m+1, r, nr);
    merge(tipo, comparison, l, r, m, nr);
}

// comparison used in the merge function to establish the order based on a specific storting key
int compare_struct(tratta *a, tratta *b, int par){
    if (par == 0) return strcmp(a->code, b->code);
    else if (par == 1) return strcmp(a->partenza, b->partenza);
    else if (par == 2) return strcmp(a->destinazione, b->destinazione);
    else if (par == 3) {
        int cmp = strcmp(a->data, b->data);
        if (cmp == 0) return strcmp(a->ora_p, b->ora_p);
        return cmp;
    }
    return 0;
}

void merge(tratta **tipo, int comparison, int l, int r, int m, int nr){
    tratta *supp[nr];
    int i = l, j = m+1, k = l;
    while (i <= m && j <= r){
        if (compare_struct(tipo[i], tipo[j], comparison) <= 0){
            supp[k] = tipo[i++];
        }
        else {
            supp[k] = tipo[j++];
        }
        k++;
    }
    while (i <= m){
        supp[k++] = tipo[i++];
    }
    while (j <= r){
        supp[k++] = tipo[j++];
    }
    for (i = l; i <= r; i++){
        tipo[i] = supp[i];
    }
}

// free all the memory allocated (theoretically)
void freeall(tratta **v, tratta ***v_data, tratta ***v_code, tratta ***v_partenza, tratta ***v_arrivo){
    free(*v);
    free(*v_data);
    free(*v_code);
    free(*v_partenza);
    free(*v_arrivo);
}