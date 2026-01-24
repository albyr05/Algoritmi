#include<stdio.h>
#include<stdlib.h>
#include "struct.h"
#include "ST.h"
#include <string.h>
#define MAXC 20
typedef enum{false, true} bool;

typedef struct node *link;
struct art{
    char titolo[MAXC];
    char relatore[MAXC];
    int slot;
    char argomento[MAXC];
}; 

struct articoli{
    ARTICOLO *a;
    ST st_art;
    int N_relators; // quando leggo gli articoli mi calcolo il numero di relatori diversi;
    int N_arguments;
    ST arguments;
    int N;
};

struct programma{
    int R;
    int S;
    int **m;        //R*S
};

// in posizione i, j della matrice c'è l'indice della ST dell'articolo presentato nell'r-esima sala nell's-esimo slot;
PROGRAMMA read_program (char *filename){
    FILE *fp = fopen(filename, "r");
    int R, S ;
    fscanf (fp, "%d %d", &R, &S);
    PROGRAMMA p = programma_init (R, S);
    for (int i = 0; i < R; i++){
        for (int j = 0; j < S; j++){
            fscanf(fp, "%d", p->m[i][j]);
        }
    }
    return p;
}

bool valid_article(int id, PROGRAMMA p, ARTICOLI A){
    int i, j, k;
    ARTICOLO curr_art = A->a[i];
    int durata = curr_art.slot;
    int start_r, start_c;
    for (i = 0; i < p->R; i++){
        for (j = 0; j < p->S; j++){
            if (p->m[i][j] == id){
                start_r = i;
                start_c = j;
            }
        }
    }
    if (start_c + durata > p->S) return false;

    for (int s = 0; s < durata; s++){
        if (p->m[start_r][start_c+s] != id) return false;
    }

    for (int s = 0; s < durata; s++){
        int curr_time = start_c + s;
        for (int j = 0; j < p->R; j++){
            if (j != start_r){
                char *rel = A->a[p->m[start_r][j]].relatore;
                if (strcmp(rel, curr_art.relatore) == 0) return false;

            }
        }
    }
    return true;
}

// assumo che gli articoli vengano usati solon una volta
bool check_program (PROGRAMMA p, ARTICOLI A){
    int i, j, k;
    bool is_valid = false;
    for (i = 0; i < A->N; i++){
        if (!valid_article(i, p, A)) return false;
    }
    return true;
}


bool can_add_article(int id, PROGRAMMA p, ARTICOLI A, int start_r, int start_c){
    int i, j, k;
    ARTICOLO curr_art = A->a[i];
    int durata = curr_art.slot;
    if (start_c + durata > p->S) return false;

    for (int s = 0; s < durata; s++){
        if (p->m[start_r][start_c+s] != id) return false;
    }

    for (int s = 0; s < durata; s++){
        int curr_time = start_c + s;
        for (int j = 0; j < p->R; j++){
            if (j != start_r){
                char *rel = A->a[p->m[start_r][j]].relatore;
                if (strcmp(rel, curr_art.relatore) == 0) return false;

            }
        }
    }
    return true;
}


int  evaluate_sol (PROGRAMMA p, ARTICOLI A){
    int i, j;
    int ind_arg = -1;
    int NA = 0;
    for (i = 0; i < p->R; i++){
        int NA_i = 0;
        for (j = 0; j < p->R; j++){
            int prev_arg = ind_arg;
            int ind_arg = STsearch(A->arguments, A->a[p->m[i][j]].argomento);
            if (ind_arg != prev_arg){
                NA_i ++;
            }

        }
        NA += NA_i ;
    }
            
}
void build_sol(int pos, ARTICOLI A, PROGRAMMA p){
    if (pos == A->N){
        evaluate_sol(p, A);
    }

    int i, j;

    for (i = 0; i < p->R; i++){
        for (j = 0; j < p->S; j++){
            if (can_add_article(pos, p, A, i, j)){
                for (int k = 0; k < A->a[pos].slot; k++){
                    p->m[i][j+k] = pos;
                }

                build_sol(pos+1, A, p);

                for (int k = 0; k < A->a[pos].slot; k++){
                    p->m[i][j+k] = -1;
                }
            }
        }
    }
}


int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}