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

int alloc_vet(FILE *fp, tratta **v);          
void ask_input (tratta *v, int *flag, tratta **v_data, tratta **v_code, tratta **v_partenza, tratta **v_arrivo, int nr);
void freall();
void print_struct (tratta **v, int nr);
void alloc_dinamic_struct(tratta *v, tratta ***v_data, tratta ***v_code, tratta ***v_partenza, tratta ***v_arrivo, int nr);
void merge_sort_pointers(tratta **tipo, int comparison, int l, int r, int nr);
void merge (tratta **tipo, int comparison, int l, int r, int m, int nr);

int main(){
    FILE *fp = fopen("lab01/es2input.txt", "r");
    if (fp == NULL) return 1;
    int flag = 0;
    tratta *vp;
    int nr = alloc_vet(fp, &vp);

    tratta **v_data, **v_code, **v_partenza, **v_arrivo;
    alloc_dinamic_struct(vp, &v_data, &v_code, &v_partenza, &v_arrivo, nr);
    while (flag == 0){
        ask_input(vp, &flag, v_data, v_code, v_partenza, v_arrivo, nr);
    }
    
    return 0;
}

 
void ask_input(tratta *v, int *flag, tratta **v_data, tratta **v_code, tratta **v_partenza, tratta **v_arrivo, int nr){
    int input; int c;
    char filename[50], path[100];

    printf ("Scegli se vuoi ordinare il vettore, leggere un nuovo input o uscire (0/1/2): ");
    scanf ("%d", &c);
    

    switch (c){
        case 0:
            printf("Scegli la chiave di ordinamento (codice[0], partenza[1], destinazione[2], data[3])");
            scanf("%d", &input);
                switch (input){
                    case 0:
                        merge_sort_pointers(v_code, input, 0, nr-1, nr);
                        print_struct(v_code, nr);
                    case 1:
                        merge_sort_pointers(v_partenza, input, 0, nr-1, nr);
                        print_struct(v_partenza, nr);
                    case 2:
                        merge_sort_pointers(v_arrivo, input, 0, nr-1, nr);
                        print_struct(v_arrivo, nr);
                    case 3:
                          merge_sort_pointers(v_data, input, 0, nr-1, nr);
                          print_struct(v_data, nr);
                    break;
                }
            break;
        
        case 1:
            printf("Inserisci il nome del file in input: ");
            scanf ("%s", filename);
            sprintf(path, "/..%s", filename);
            FILE *newf = fopen(path, "r");
            if (newf == NULL) return;

            //freeall(vp);
            nr = alloc_vet (newf, v);
            
            break;

        case 2:
            *flag = 1;
    }
}

int alloc_vet(FILE *fp, tratta **v){
    int nr;
    fscanf(fp, "%d", &nr);
    tratta *v_help;
    v_help = malloc (nr*sizeof(tratta));
    for (int i = 0; i < nr; i++){
        fscanf (fp, "%s %s %s %s %s %s %d", v_help[i].code, v_help[i].partenza, v_help[i].destinazione, v_help[i].data, v_help[i].ora_p, v_help[i].ora_a, &v_help[i].ritardo);
    }
    *v = v_help;
    return nr;
}

void print_struct(tratta **v, int nr){
    for (int i = 0; i < nr; i++){
        printf("%s %s %s %s %s %s %d\n", v[i]->code, v[i]->partenza, v[i]->destinazione, v[i]->data, v[i]->ora_p, v[i]->ora_a, v[i]->ritardo);
    }
}


void alloc_dinamic_struct(tratta *v, tratta ***v_data, tratta ***v_code, tratta ***v_partenza, tratta ***v_arrivo, int nr){
    tratta **v_data_help = malloc(nr*(sizeof(tratta*)));
    tratta **v_code_help = malloc(nr*(sizeof(tratta*)));
    tratta **v_partenza_help = malloc(nr*(sizeof(tratta*)));
    tratta **v_arrivo_help = malloc(nr*(sizeof(tratta*)));

    for (int i = 0; i < nr; i ++){
        v_data_help[i] = &v[i];
        v_code_help[i] = &v[i];
        v_partenza_help[i] = &v[i];
        v_arrivo_help[i] = &v[i];
    }

    *v_data = v_data_help;
    *v_code = v_code_help;
    *v_partenza = v_partenza_help;
    *v_arrivo = v_arrivo_help;

}

void merge_sort_pointers(tratta **tipo, int comparison, int l, int r, int nr){
    if (l>=r){
        return;
    }
    int m = (l+r) / 2;
    merge_sort_pointers(tipo, comparison, l, m, nr);
    merge_sort_pointers(tipo, comparison, m+1, r, nr);
    merge(tipo, comparison, l, r, m, nr);
}

int compare_struct(tratta *a, tratta *b, int par){
    if (par == 0) {
        if (strcmp(a->data, b->data) == 0){
            return strcmp(a->ora_p, b->ora_p);
        }
        return strcmp(a->data, b->data);
    }
    else if (par == 1) return strcmp(a->code, b->code);
    else if (par == 2) return strcmp (a->partenza, b->partenza);
    else if (par == 3) return strcmp (a->destinazione, b->destinazione);
    return 0;
}


void merge(tratta **tipo, int comparison, int l, int r, int m, int nr){
    tratta *supp[nr];
    int i = l; int j = m+1, k = l;
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
    for (i = l; i < nr; i++){
        tipo[i] = supp[i];
    }
}