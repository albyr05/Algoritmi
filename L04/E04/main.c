#include <stdio.h>
#include <stdlib.h>

// global variables useful to know next stone's rules
int stones_relations[4][2] = {{0, 1}, {2, 3}, {1, 0}, {2,3}};
char index_to_stone [4] = "zrts"; // useful to print;

void calculate_necklace(int pos, int current_val, int *sol, int *stones_N, int *stones_values, int maxrep, int *maxvalue, int total_stones, int *streak, int *len, int *best_sol){
    int i, j, new_streak;
    
    if (current_val > *maxvalue) {      
        *maxvalue = current_val;        // updating the new max value if the one of the current building necklace is greater 
        for (i = 0; i < pos; i++){          // saving the current best solution in a vector to print it at the end 
            best_sol[i] = sol[i];
        }
        *len = pos;         // saving alse the length of the new best solution founud 

    }

    int max_possible_val_brench = 0;        // calculating the max possibile value reachable in the current brench 
    for (i = 0; i < 4; i ++){
        max_possible_val_brench += (stones_N[i]*stones_values[i]);  // formula to calculate this value 
    }
    if (current_val + max_possible_val_brench <= *maxvalue) return;     // if this value, just calculated, is less that the current maxvalue -> means that it can't be beated from this brench so break it 
    
    if (pos >= total_stones) return ;       // all the stones have been placed 

    if (pos != 0){  
        for (j = 0; j < 2; j++){
            int candidate = stones_relations[sol[pos-1]][j];            // choosing the next candidate stone based on the previous one (by matrix access)
            if (stones_N[candidate] > 0){
                int n_z = 0, n_s = 0;
                if (candidate == 0){
                    for (int k = 0; k < pos; k++){          // counting the number of zaffires and emeralds in the current necklace 
                        if (sol[k] == 0) n_z ++;
                        if (sol[k] == 3) n_s++;
                    }
                    if (n_z+1 > n_s) continue;  // n_z +1 beacuse i'm in the case of inserting a zaffire: if the condition is true I can move on selecting an another new stone
                }

                if (candidate == sol[pos-1]){       // if two subsequent elements are equal increment the streak
                    new_streak = *streak + 1;
                }
                else {new_streak = 1;}      // resetting the streak to one 

                if (new_streak > maxrep){              // if it is greater try placing a new stone 
                    continue;
                }
                
                sol[pos] = candidate;       // if all the checks are ok i can place the candidate stone
                stones_N[candidate] --;         // eliminating the insert stone
                current_val += stones_values[candidate];            // adding the insert stone value to the current val
                calculate_necklace(pos+1, current_val, sol, stones_N, stones_values, maxrep, maxvalue, total_stones, &new_streak, len, best_sol);       // recalling the new recursive func
                stones_N[candidate] ++;         // backtracking so replace back the new stone 
                current_val -= stones_values[candidate];            // decrementing the value 
            }
        }
    }

    else{           // same logic 
        for (i = 0; i < 4; i++){
            if (stones_N[i] > 0){ 
                sol[pos] = i;
                if (i == 0) continue;       // the first stone can't be a zaffiire
                stones_N[i] --;         
                current_val = stones_values[i];
                new_streak = 1;             // initializing the streak to one
                calculate_necklace(pos+1, current_val, sol, stones_N, stones_values, maxrep, maxvalue, total_stones, &new_streak, len, best_sol);
                stones_N[i] ++;
            }
        }
    }
    return;

}

int sol_wrapper (int *stones_N, int *stones_values, int max_rep){
    int total_stones = 0, i;            
    for (i = 0; i < 4; i ++){
        total_stones += stones_N[i];        // calculating the total number of stones in the read line
    }
    int *sol = malloc(total_stones*sizeof(int));        // allocating the temporary sol vector 
    int max_value = 0, streak = 0;
    int len = 0;
    int *best_sol = malloc(total_stones*sizeof(int));       // allocating the vector for the best sol at the end 

    calculate_necklace(0, 0, sol, stones_N, stones_values, max_rep, &max_value, total_stones, &streak, &len, best_sol);  
    for (i = 0; i < len; i ++){
        printf("%c ", index_to_stone[best_sol[i]]);     // print the best sol 
    }
    printf(" len = %d", len);
    free(sol);
    free(best_sol);
    return max_value;           
}


void readfile (FILE *fp){
    int nr, i, j;
    int max_rep;
    fscanf(fp, "%d", &nr);
    int *stones_N = malloc(4*sizeof(int));
    int *stones_values = malloc(4*sizeof(int));
    int max_val;
    for (i = 0; i < nr; i++){
        for (j = 0; j < 4; j ++){           // reading the stone from the current line 
            fscanf(fp, "%d", &stones_N[j]);
        }
        for (j = 0; j < 4; j++){
            fscanf(fp, "%d", &stones_values[j]);
        }
        fscanf(fp, "%d", &max_rep);
        printf("SOLUZIONE OTTIMALE RIGA %d:  ", i+1);
        max_val = sol_wrapper(stones_N, stones_values, max_rep);
        printf("    valore massimo (%d)\n", max_val);
    }
    

    free(stones_N);
    free(stones_values);
    
}

int main(){
    FILE *fp = fopen("test.txt", "r");
    readfile(fp);
    fclose(fp);
    return 0;
}