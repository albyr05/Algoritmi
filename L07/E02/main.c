#include<stdio.h>
#include<stdlib.h>

#define DD 10
#define DP 20
#define ME 5
#define MD 3
typedef struct {
    char name[100];
    int type;
    int entry;
    int exit;
    int priority;
    int final;
    float value;
    int difficulty;
    
}element;

typedef enum{false, true} bool;

//allocating 3D matrix for solution, setting all the pointers to NULL, in order to know the exact length of solution 
void alloc_matrix(element ****m){
    element ***solution = malloc(MD*sizeof(element**));
    for (int i = 0; i < MD; i++){
        solution[i] = malloc(ME*sizeof(element*));
        for (int j = 0; j < ME; j++){
            solution[i][j] = NULL;
        }
    }
    *m = solution;
}
//freeing the solution
void free_solution(element ***m){
    for (int i = 0; i<MD; i++){
        free(m[i]);
    }
    free(m);
}

// checking if an element can be insered in a determined position 
bool check_valid(int pos, element candidate, element **diag, int current_diff_diag){
    if (pos == 0){
        if (candidate.entry == 1 && candidate.priority == 0 && current_diff_diag + candidate.difficulty <= DD) return true; // front entrance condition, no priority and DD bound 
        return false;
    }
    if (candidate.entry == diag[pos-1]->exit && current_diff_diag + candidate.difficulty <= DD) return true; // compatibility for entrance/exit condition

    return false; // not valid candidate
}

// validating the program solution and getting its score
bool check_solution(element ***solution, float *program_value){
    int i = 0, j;
    bool front = false, back = false, sequence = false;
    int diff = 0; 
    while (i < MD){
        j = 0;
        float diag_value= 0;
        while(j < ME && solution[i][j] != NULL){
            if (j > 0 && solution[i][j]->type != 0 && solution[i][j-1]->type != 0) sequence = true;     // checkint that at least one diagonal has two acro in sequence
            if (solution[i][j]->type == 2) front = true;        // an element has to be front acro
            if (solution[i][j]->type == 1) back = true;         // at least an element has to be back acro

            diff += solution[i][j]->difficulty;     // calculating the total diff of the entire program
            diag_value += solution[i][j]->value;           // calculating the point of the current diag
            j ++;
        }
        if (i == 2 && j > 0 && solution[i][j-1]->difficulty >= 8) {
              // checking if third diag ends with diff >= 8 --> multipling points
                *program_value += (diag_value * 1.5);
            
        } 
        else 
            *program_value += diag_value;       // adding the to total score 
        i++;
    }
    
    // not valid solution condition 
    if (diff > DP) return false;
    if (!(front && back)) return false;
    if (!sequence) return false;

    // valid sol
    return true;
}


// recursive function on 3 level, one for each diagonal
void layer (int pos, element *v, int current_diff_diag, float *max_value, bool has_acro, int nr, element ***solution, element ***bestsolution, int current_layer, int total_program_diff){
    int i = 0;
    if (current_layer == MD){   // means i have completed the third diagonal and so i can check if the program is valid and calculating its score
        float program_value = 0;
        if (check_solution(solution, &program_value)){
            if(program_value > *max_value){     // found a new best solution
                *max_value = program_value;     // updating the new top score
                int j, k = 0;
                // copying the solution
                while (k < MD){
                    j = 0;
                    while (j < ME && solution[k][j] != NULL){
                        bestsolution[k][j] = solution[k][j];        // copying the pointers to the element in *v
                        j++;
                    }
                    while (j < ME){ 
                        bestsolution[k][j] = NULL;      // resetting the empty slots to NULL
                        j++;
                    }
                    k++;
                }
            }
        }
        return;
    }

    
    if (pos >= ME) return;      // the diag is full length
    
    
    if (pos > 0 && has_acro) {      // if the diag has at least one element and an acro i can start the next diag 
        layer(0, v, 0, max_value, false, nr, solution, bestsolution, current_layer+1, total_program_diff + current_diff_diag);     // setting the diag specific value to zero and incrementing the layer
    }
    
    if (current_diff_diag > DD) return;     // the current diag diff is higher than the bound --> cutting the brench

    
    if (total_program_diff + current_diff_diag > DP) return;    // cutting the brench if the value is already higher than DP



    for (i = 0; i < nr; i++){       //trying all elements
        
        if (check_valid(pos, v[i], solution[current_layer], current_diff_diag)){        // checking the validity of the insertion
            solution[current_layer][pos] = &v[i];       // making the solution pointer pointing to the element in th vector 
            current_diff_diag += v[i].difficulty;           // adding the value and difficulty of the current diag 
            //current_value += v[i].value;

            bool old_has_acro = has_acro;       // saving the current flag, useful in backtracking
            if (v[i].type != 0) has_acro = true;        // an acro has been insert 

            if (v[i].final == 1){       // reached a ending element --> has to go to next layer
                layer(0, v, 0, max_value, false, nr, solution, bestsolution, current_layer+1, total_program_diff + current_diff_diag);
            }
            // continuing in the current layer adding new elements to the diag 
            else{
                layer(pos+1, v, current_diff_diag,  max_value, has_acro, nr, solution, bestsolution, current_layer, total_program_diff);
            }

            //BACKTRACK
            current_diff_diag -= v[i].difficulty;
            //current_value -= v[i].value;
            solution[current_layer][pos] = NULL;        // resetting the pointer to NULL for future solution
            has_acro = old_has_acro;        // restoring the flag to the value in the current call
        }

    }
    return;
}



void wrapper_best_program(element *v, int nr){
    element ***solution;
    element ***best_solution;
    alloc_matrix(&solution);
    alloc_matrix(&best_solution);
    float max_value = 0.0;
    layer(0, v, 0, &max_value, false, nr, solution, best_solution, 0, 0);
    int i = 0, j;
    printf("SOLUZIONE DI PUNTEGGIO MASSIMO (%f)\n", max_value);
    while(i<MD){
        j = 0;
        printf("DIAGONALE %d:\n", i+1);
        while (j < ME && best_solution[i][j] != NULL){      // printing element until NULL pointers, allows me to not have count of any length
            printf("    %s \n", best_solution[i][j]->name);
            j++;
        }
        printf("\n\n");
        i++;
    }   
    free_solution(solution);
    free_solution(best_solution);
}

// loading the vector of elements from file
int readfile(element **v){
    FILE *fp;
    if ((fp = fopen("elementi.txt", "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "elementi.txt");
        exit(-1);
    }
    int nr;
    fscanf(fp, "%d", &nr);
    element *vector = malloc(nr*sizeof(element));
    for (int i = 0; i < nr; i++){
        fscanf(fp, "%s %d %d %d %d %d %f %d", vector[i].name, &vector[i].type, &vector[i].entry, &vector[i].exit, &vector[i].priority, &vector[i].final, &vector[i].value, &vector[i].difficulty);
        
    }
    *v = vector;
    fclose(fp);
    return nr;
}
int main(int argc, char* argv[]){
    element *v;
    int nr = readfile(&v);
    wrapper_best_program(v, nr);
    free(v);
    printf("\n");
    return 0;
}