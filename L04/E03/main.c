#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;

// matrix to define subsequently stones' rules each row contains the allowed stones for the one in that row 
// example: zaffire (corresponding index 0) --> row 0 contains {0,1} because it can be followed only by another zaffire (0) or rubin (index 1) and so on.
int stones_relations[4][2] = {{0, 1}, {2, 3}, {1, 0}, {2,3}};

// FUNCTION FIRSTLY BUILT TO DO A BIT OF PRUNING BUT NOT ENOUGH

// bool check_valid(int curr, int succ){
//     if (succ == stones_relations[curr][0] || succ == stones_relations[curr][1]) return true;
//     return false;
// }


// WITH THIS IMPLEMENTATION I'M GETTING FAST SOLUTION (really good pruning)
int calculate_necklace(int pos, int *sol, int *stones_N, int *maxlen, int N_total){
    int i, j;
    int l = 0;
    
    if (pos > *maxlen) *maxlen = pos;           // pos is the current length of the necklace --> updating maxlength 

    // REALLY USEFUL CHECK TO SAVE RECURSIVE CALLS AND TIME EXECUTION
    int remaing = 0;
    for (int k = 0; k < 4; k++){            // counting the remaining stones 
        remaing += stones_N[k];
    }
    if (pos + remaing <= *maxlen) return *maxlen;       // checking the current brench, even with all the stones palced correctly, can reach the maxlength --> pruning if not

    if (pos >= N_total) return *maxlen;     // if I reach max length we are out of stones so returns 

    
    if (pos != 0){
        for (j = 0; j < 2; j++){            // exploring only the possible brenches based on stones raltions
            i = stones_relations[sol[pos-1]][j];             
            if (stones_N[i] > 0){               // checking if there still are this type of stone
                if (i == 0 || i == 3){                  // if im inserting a zaffire or emerald (which can be followed by themselves)
                    for (;stones_N[i] > 0; pos++, stones_N[i]--, l++){          // i'm inserting al those stones in a row to save a lot of recursive call
                        sol[pos] = i;
                    }
                    calculate_necklace(pos, sol, stones_N, maxlen, N_total);        // then i can go on with recursion
                    for (; l > 0; pos--, stones_N[i]++, l--);       // backtracking to remove these stones
                }
                else{
                    sol[pos] = i;              // insert the stones 
                    stones_N[i] --;         // decrementing the supply of the insert stone
                    calculate_necklace(pos+1, sol, stones_N, maxlen, N_total);      // recursive call to go onwards in necklace building 
                    stones_N[i] ++;             // BACKTRACK: puts back the stone 
                }
            }
        }
    }

    else {          // pos == 0 --> all the choices are allowed, logic as the other cases
        for (i = 0; i < 4; i++){
            if (stones_N[i]>0){
                sol[pos] = i;
                stones_N[i] --;
                calculate_necklace(pos+1, sol, stones_N, maxlen, N_total);
                stones_N[i]++;
            }
        }
    }


    return *maxlen;
}


int create_stones_set(int *stones_N){
    int N_total = 0, i;
    for (i = 0; i < 4; i ++){
        N_total += stones_N[i];
    }
    int maxlen = 0;
    int *sol = malloc(N_total*sizeof(int));     // allocating a maxlen solution array
    maxlen = calculate_necklace(0, sol, stones_N, &maxlen, N_total);
    for (i = 0; i < maxlen; i++){
        printf("%d ", sol[i]);      // printing the optimal solution after recursion
    }
    free(sol);      // freeing heap space
    return maxlen;
}
    
void readfile(FILE *fp){
    int nr, i, j, k;

    int *stones_N = malloc(4 *sizeof(int));
    fscanf(fp, "%d", &nr);
    for (i = 0; i < nr; i++){           // reading each line of the file 
        for (j = 0; j < 4; j++){
            fscanf(fp, "%d", &stones_N[j]);
        }
        printf("SOLUZIONE OTTIMALE RIGA %d:     ", i+1);        // PRINTNG OOPTIMAL SOLUTION AND ITS LENGTH
        k = create_stones_set(stones_N);
        printf("    len(%d)", k);
        printf("\n");
        
    }
    free(stones_N);     

    fclose (fp);
    
}

int main(){
    FILE *fp = fopen("easytest.txt", "r");
    readfile(fp);
    printf("\n\nPROGRAMMA TERMINATO");
    return 0;
}