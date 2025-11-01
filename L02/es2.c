#include <stdio.h>
#include <stdlib.h>

int **malloc2dr (int nr, int nc);
void print_matrix(int ** m, int nr, int nc, int *w, int *b, int sizeW, int sizeB);
void free2d(int **m, int nr);
void separa (int **m, int nr, int nc, int **white, int **black);
void malloc2P (int ***mat, int nr, int nc);
//------------------------------------------------------------------------------------------------------

int main(){
    FILE *fp = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab02/es2input.txt", "r");
    int nr, nc, choice;
    fscanf(fp, "%d %d", &nr, &nc);
    printf ("matrice acquisita by reference o con volore di ritorno (0/1): ");
    
    scanf("%d", &choice);
    int **matrix;
    switch (choice){
        case 0: {
            matrix = malloc2dr(nr, nc);    // allocazione con ritorno
            break;
        }
        case 1: {
            malloc2P(&matrix, nr, nc);      // allocazione by reference 
            break;
        }
    }

    int i, j;
    for (i = 0; i < nr; i++){
        for (j = 0; j < nc; j++){
            fscanf(fp, "%d", &matrix[i][j]);
        }
    }
    int *white = NULL, *black = NULL;
    int sizeW = (nr*nc+1) / 2, sizeB = nr*nc/2;
    separa(matrix, nr, nc, &white, &black);

    print_matrix(matrix, nr, nc, white, black, sizeW, sizeB);

    free2d(matrix, nr);             // libero matrice
    free(white); free(black);           // libero i due vettori dinamici
}

// allocazione dinamica della matrice
int **malloc2dr(int nr, int nc){
    int **m, i;
    m = malloc(nr*(sizeof(int *)));
    for (i = 0; i < nr; i++){
        m[i] = malloc(nc*(sizeof(int)));
    }
    return m;
}

// malloc by reference
void malloc2P(int ***mat, int nr, int nc){
    int **m;
    m = malloc(nr*sizeof(int*));
    for (int i = 0; i < nc; i++){
        m[i] = malloc(nc*sizeof(int));
    }
    *mat = m;
}

// stampo l'output
void print_matrix(int ** m, int nr, int nc, int *w, int*b, int sizeW, int sizeB){
    int i, j;
    printf("Stampa della matrice\n");
    for (i = 0; i < nr; i++){
        for (j = 0; j < nc; j++){
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }

    printf("\ncaselle bianche: ");
    for (i = 0; i < sizeW; i ++){
        printf ("%d ", w[i]);
    }
    printf("\ncaselle nere: ");
    for (i = 0; i < sizeB; i ++){
        printf ("%d ", b[i]);
    }

}

// libero la matrice
void free2d(int **m, int nr){
    int i;
    for (i = 0; i < nr; i++){
        free(m[i]);
    }
    free(m);
}


void separa(int **m, int nr, int nc, int **white, int **black){
    int i, j, iw = 0, ib = 0;
    int sizeW = (nr*nc + 1) / 2;            // calcolo le caselle nere e bianche
    int sizeB = (nr*nc) / 2;
    
    int *w = (int *) malloc(sizeW*sizeof(int));     // alloco i 2 vettori dinamici
    int *b = (int *) malloc(sizeB*sizeof(int));

    for (i = 0; i < nr; i++){
        for (j = 0; j < nc; j ++){
            if ((i+j) % 2 == 0){
                w[iw++] = m[i][j];
            }
            else b[ib++] = m[i][j];
        }
    }

    *white = w; *black = b;     // passo il puntatore ai vettori della main
}