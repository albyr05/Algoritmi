#include<stdio.h>
#include<stdlib.h>


// APPROACH EXPLAINATION
// with the greedy approach we can't reach the optimal solution so, that's what i figured it out to find at least a valid one
// in order to respect all the constraints given by the problem i made some choices and i satisfy them one at a time
// starting with the sorting of the array i chose the value/difficulty ratio, because i have to maximise the total score keeping the the difficulty low
// in the first diagonal i always put two acrobatic elements in a row in order to immediately satisfy this condition, and one of them has to be front acro 
// in the second diagonal i look for back acro element and then try to find a previous one that can start the diagonal and that respect the entry/condition as well
// now all the general constrains are satisfy and i can do all in on the third diagonal
// i just put two elements in each diagonal so far so i maximize the chances of having space for diff >= 8 element in the third diagonal to obtain the 1.5x score bonus
// infact i build the third diagonal as follow:
// first thing first i check for a diff>= 8 element to insert that can still respect the difficulties bounds 
// i look for previous compatibile elements to add and check in the end if the one that should be first can actually be so if not i check for an other elements
// if i couldn't build a diagonal with an element with diff >= 8 i proceed with the building from the start checking the compatibility between elements and insert them
// check the comments in the code for more specifics
#define DD 7
#define DP 35
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

void alloc_sol(element ***matr){
    element **m = malloc(MD*sizeof(element*));
    for (int i = 0; i < MD; i++){
        m[i] = malloc(ME*sizeof(element));
    }
    *matr = m;
}

// compare used to sort elements by value /difficulty ratio
int compare(element a, element b){
    if (a.value / (float) a.difficulty > b.value / (float) b.difficulty) return 1;
    else if (a.value / (float) a.difficulty < b.value / (float) b.difficulty) return -1;

    if (a.type != 0) return 1;
    else return -1;
}


void merge_s(element *v, int l, int m, int r){
    int k = 0;
    int i = l, j = m+1;
    element *tmp = malloc((r-l+1)*sizeof(element));

    while (i <= m && j <= r){
        if (compare(v[i], v[j]) > 0)
            tmp[k++] = v[i++];
        else
            tmp[k++] = v[j++];
    }
    while (i <= m) tmp[k++] = v[i++];
    while (j <= r) tmp[k++] = v[j++];

    for (i = l; i <= r; i++)
        v[i] = tmp[i - l];

    free(tmp);

}


void merge_sort(element *v, int l, int r){
    if (l >= r) return ;
    int m = (l+r)/2;
    merge_sort(v, l, m);
    merge_sort(v, m+1, r);

    merge_s(v, l, m, r);
}

// matrix used to check if two elements are compatibile in entry/exit conditions
void compatibility_matrix(int ***comp, int nr, element *v){
    int **m = malloc(nr*sizeof(int*));
    for (int i = 0; i < nr; i ++){
        m[i] = calloc(nr, sizeof(int));
    }
    int i, j;
    for (i = 0; i < nr; i ++){
        for (j = 0; j < nr; j++){
            if (v[i].exit == v[j].entry ){
                m[i][j] = 1;
            }
        }
    }
    *comp = m; 
}   

// general controls
bool check_valid(int pos, element candidate, int current_diff_diag, int total_diff){
    if (pos == 0){
        if (candidate.entry == 1 && candidate.priority == 0 && current_diff_diag + candidate.difficulty < DD && total_diff + candidate.difficulty < DP) return true; // front entrance condition, no priority and DD bound 
        return false;
    }
    if (current_diff_diag + candidate.difficulty < DD && total_diff + candidate.difficulty < DP) return true; // compatibility for entrance/exit condition

    return false; // not valid candidate
}


void create_first_diag(element *first_diag, element *v, int nr, int **comp_matrix, int *total_diff, float *score){
    int i, j, k, current_diff = 0;
    i = 0;
    bool filled = false;
    
    for (j = 0; j < nr && !filled; j++){
        if (check_valid(i, v[j], current_diff, *total_diff) && v[j].final != 1 && v[j].type != 0){      // looking for a first acro elements 
            for (k = 0; k < nr && !filled; k ++){
                if (comp_matrix[j][k] == 1 && v[k].type != 0 && (v[k].type == 2 || v[j].type == 2) && v[j].difficulty + v[k].difficulty < DD){      // looking for a compatibile acro elements (at least one of them has to be front acro)
                    first_diag[i++] = v[j]; first_diag[i++] = v[k];     // insert elements and update global value
                    (*total_diff) += v[j].difficulty + v[k].difficulty;
                    (*score) += v[j].value + v[k].value;
                    filled = true;  
                }
            }
        }
    }
    if (!filled) return;
    printf("\nFirst diagonal:\n");
    for (k = 0; k < i; k++){
        printf("%s %.2f %d\n", first_diag[k].name, first_diag[k].value, first_diag[k].difficulty);
    }
}


void create_second_diag(element *second_diag, element *v, int nr, int **comp_matrix, int *total_diff, float *score){
    int i, j, k, current_diff = 0;
    int lowest_diff = DD;
    float partial_score = 0.0;
    i = 0;
    bool filled = false;
    for (j = 0; j < nr; j++){       // forcing two elements (the second has to be back acro)
        if (v[j].type == 1){
            for (k = 0; k < nr; k++){
                if (comp_matrix[k][j] == 1 && check_valid(i, v[k], v[j].difficulty, (*total_diff) + v[j].difficulty)){
                    if(v[j].difficulty + v[k].difficulty < lowest_diff){        //choosing the two with lower difficulty to increase the chances of having space for diff >= 8 in the third diagonal
                        i = 0; partial_score = 0.0;
                        lowest_diff = v[j].difficulty + v[k].difficulty;
                        second_diag[i++] = v[k]; second_diag[i++] = v[j]; 
                        partial_score += v[j].value + v[k].value;
                    }
                }   
            }
        }
    }
    (*total_diff) += lowest_diff;
    (*score) += partial_score;
    
    printf("\nSecond diagonal:\n");
    for (k = 0; k < i; k++){
        printf("%s %.2f %d\n", second_diag[k].name, second_diag[k].value, second_diag[k].difficulty);
    }
}



void create_third_diag(element *third_diag, element *v, int nr, int **comp_matrix, int *total_diff, float *score){
    int i, j, k, current_diff, l;
    int index;
    bool filled, found_target = false;
    float partial_value;
    bool acro, canbefirst;
    
    // trying to build a diagonal with element diff >= 8
    for (l = 0; l < nr && !found_target; l++){
        // Reset at each try
        i = ME-1;
        current_diff = 0;
        partial_value = 0;
        acro = false;
        canbefirst = false;
        
        // base controls
        if (v[l].difficulty >= 8 && v[l].difficulty < DD && v[l].difficulty + (*total_diff) < DP){
            
            // checking if the element can possibly be first 
            if (check_valid(0, v[l], 0, *total_diff)) canbefirst = true;
            
            // putting it last 
            current_diff = v[l].difficulty;
            partial_value = v[l].value;
            if (v[l].type != 0) acro = true;
            index = l;
            
            third_diag[i] = v[l];  
            i--;
            
            // trying compatibile elements before it 
            while (i >= 0 && current_diff < DD && (*total_diff) + current_diff < DP){
                filled = false;
                
                for (j = 0; j < nr && !filled; j++){
                    if (check_valid(i, v[j], current_diff, *total_diff) && comp_matrix[j][index] == 1 && v[j].final != 1 && (acro || v[j].type != 0)){
                        
                        if (v[j].type != 0) acro = true;
                        third_diag[i] = v[j]; 
                        current_diff += v[j].difficulty;
                        partial_value += v[j].value;
                        filled = true;
                        i--;
                        index = j;
                    }
                }
                if (!filled) break; // no more elements can be added 
            }
            
            // checking the diagonal
            if (i < ME-1 && acro){  // acro condition
                if (i == ME-2){  // i have insert only the diff >= 8 element so i check if it can be first
                    if (canbefirst){
                        found_target = true;
                    }
                } 
                else {  
                    // more elements inserted so i check the first one
                    if (check_valid(0, third_diag[i+1], 0, *total_diff)){
                        found_target = true;
                    }
                }
            }
        }
    }
    
    //  prints the valid diag 
    if (found_target){
        (*total_diff) += current_diff;
        (*score) += partial_value * 1.5;  // Bonus 1.5x
        printf("\nThird diagonal:\n");
        for (k = i+1; k < ME; k++){
            printf("%s %.2f %d \n", third_diag[k].name, third_diag[k].value, third_diag[k].difficulty);
        }
    }


    // COULDN'T BUILD WITH DIFF >= 8
    else {
        // build normal diag
        i = 0;
        current_diff = 0;
        partial_value = 0;
        acro = false;
        
        // insert the first element
        for (j = 0; j < nr; j++){
            if (check_valid(0, v[j], 0, *total_diff)){
                if (v[j].type != 0) acro = true;
                third_diag[i++] = v[j]; 
                current_diff += v[j].difficulty;
                partial_value += v[j].value;
                index = j;
                break;
            }
        }
        
        // next elements
        while (i < ME && current_diff < DD){
            filled = false;
            
            for (j = 0; j < nr && !filled; j++){
                if (comp_matrix[index][j] == 1 && 
                    check_valid(i, v[j], current_diff, *total_diff) && (acro || v[j].type != 0)){
                    
                    third_diag[i] = v[j]; 
                    current_diff += v[j].difficulty;
                    partial_value += v[j].value;
                    if (v[j].type != 0) acro = true;
                    index = j;
                    filled = true;
                }
            }
            i++; 
            if (!filled) break; // no more can be added
        }
        
        (*total_diff) += current_diff;
        (*score) += partial_value;
        // print diagonal
        printf("\nThird diagonal:\n");
        for (k = 0; k < i-1; k++){
            printf("%s %.2f %d\n", third_diag[k].name, third_diag[k].value, third_diag[k].difficulty);
        }
    }
}
    
    


void greedy_approach(element *v, int nr){
    element ** sol;
    int **comp_matrix;
    compatibility_matrix(&comp_matrix, nr, v);
    int total_diff = 0;
    float score = 0;
    bool front_acro = false, back_acro = false;
    alloc_sol (&sol);
    create_first_diag(sol[0], v, nr, comp_matrix, &total_diff, &score);
    create_second_diag(sol[1], v, nr, comp_matrix, &total_diff, &score);
    create_third_diag(sol[2], v, nr, comp_matrix, &total_diff, &score);
    printf("\nBest score: %f    Total_diff: %d/%d", score, total_diff, DP);
    for (int i = 0; i < MD; i++){
        free(sol[i]);
    }
    free(sol);
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
    merge_sort(v, 0, nr-1);
    greedy_approach(v, nr);
    free(v);
    printf("\n");
    return 0;
}