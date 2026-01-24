#include<stdio.h>
#include<stdlib.h>
#include "struct.h"
#include <string.h>
typedef enum{false, true} bool;

typedef struct {
    char character;
    int *word_passing;
    int N_words;
}cell;

struct word_s {
    char word[15];
    int value;
    int sr; 
    int sc;
    int er;
    int ec;
};

struct solution{
    char **sol_words;
    int N_words;
    int sol_value;
};

struct grid {
    cell **m;
    int R;
    int C;
};

GRID read_grid (char * filename){
    FILE *fp = fopen(filename, "r");
    int R, C;
    fscanf (fp, "%d %d", &R, &C);
    GRID G = alloc_grid (R, C);
    int i,j;
    for (i = 0; i < R; i++){
        for (j = 0; j < C; j++){
            fscanf (fp, "%c", &G->m[i][j].character);
        }
    }
    return G;
}

int read_parole (char *file_words, WORD *words){
    FILE *fp = fopen(file_words, "r");
    char buffer[15]; int val;
    int i = 0;
    while (fscanf(fp, "%s %d", buffer, &val) == 2){
        strcpy (words[i]->word, buffer);
        words[i]->value = val;
        i++;
    }
    return i;
}

bool check_direction (char *curr_p, int dr, int dc, int curr_r, int curr_c, GRID G){
    if (*curr_p == '\0') return true;

    if (curr_r + dr >= G->R || curr_c + dc >= G->C) return false; // out of boundaries

    if (*curr_p != G->m[curr_r + dr][curr_c + dc].character) return false;

    if (check_direction (curr_p += 1, dr, dc, curr_r + dr, curr_c + dc, G)) return true;

    return false;
}


bool no_sovrapposition(WORD word_to_check, WORD *words, int N_words, GRID G){
    bool check = false;
    
    int i = 0, j, k;
    for (i = 0; i < N_words; i++){
        if (words[i]->sr == -1) continue;
        if (word_to_check->sr < words[i]->sr || word_to_check->sc < words[i]->ec || word_to_check->sr < words[i]->er){
            for (j = 0; j < strlen(word_to_check)-1; j++){
                for (k = 0; k < strlen(words[i]->word)-1; k++){
                    if (word_to_check->word[j] == words[i]->word[k] && word_to_check->word[j+1] == words[i]->word[k+1]){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


bool check_valid (WORD word_to_check, WORD *words, int N_words, GRID G){
    int i, j;
    bool present = false;

    for (i = 0; i < N_words && !present; i++){
        if (strcmp(words[i]->word, word_to_check)) {
            word_to_check->value = words[i]->value;
            present = true;
        }
    }   
    
    if (!present) return false;

    char *p = word_to_check;

    bool is_valid = false;
    int dr[3] = {1, 0, 1};
    int dc[3] = {0, 1, 1};
    for (i = 0; i < G->R && !is_valid; i++){
        for (j = 0; j < G->C && !is_valid; j++){
            if (G->m[i][j].character == *p){
                for (int k = 0; k < 3 && !is_valid; k++){
                    if (check_direction (word_to_check->word[0], dr[k], dc[k], i, j, G)){
                        word_to_check->sr = i;
                        word_to_check->sc = j;
                        word_to_check->er = i + dr[k]*strlen(word_to_check->word);
                        word_to_check->ec = j + dr[k]*strlen(word_to_check->word);
                        is_valid = true;
                        if (!no_sovrapposition(word_to_check)){
                            word_to_check->sr = -1;
                            word_to_check->sc = -1;
                            word_to_check->er = -1;
                            word_to_check->ec = -1;
                            is_valid = false;
                        }
                    }
                }
            }
        }
    }
    return !is_valid;
}


bool read_proposal (char *file_prop, GRID G, WORD *words){
    FILE *fp = fopen (file_prop, "r");
    int N_words;
    fscanf (fp, "%d", N_words);
    int i = 0;
    char curr_word[15];
    WORD is_to_check;

    for (i = 0; i < 0; i++){
        fscanf (fp,"%s", curr_word);
        strcpy (is_to_check->word, curr_word);
        is_to_check->sr = -1; 
        check_valid(curr_word, words, N_words, G);
    }
}



void recursive(int pos, WORD *words, int N_words, GRID G, int *solution, int *best_sol, int curr_val, int *bestval, int *LEN){
    int i, j, k;
    if (curr_val >= *bestval){
        if (pos > *LEN){
            for (i = 0; i < pos; i++){
                best_sol[i] = solution[i]; 
                *bestval = curr_val;
                *LEN = pos;
            }
        }
        return;
    }
    
    recursive (pos+1, words, N_words, G, solution, best_sol, curr_val, bestval, LEN);

    int dr[3] = {1, 0, 1};
    int dc[3] = {0, 1, 1};
    for (i = 0; i < G->R; i++){
        for (j = 0; j < G->C; j++){
            if (G->m[i][j].character == *p){
                for (int k = 0; k < 3; k++){
                    if (check_direction (words[pos]->word[0], dr[k], dc[k], i, j, G)){
                        words[pos]->sr = i;
                        words[pos]->sc = j;
                        words[pos]->er = i + dr[k]*strlen(words[pos]->word);
                        words[pos]->ec = j + dr[k]*strlen(words[pos]->word);
                        
                        if (!no_sovrapposition(words[pos], words, N_words, G)){
                            words[pos]->sr = -1;
                            words[pos]->sc = -1;
                            words[pos]->er = -1;
                            words[pos]->ec = -1;
                        }
                        
                        recursive(pos+1, words, N_words, G, solution, best_sol, curr_val+words[pos]->value, bestval, LEN)
                    }
                }
            }
        }
    }
    


    
}
int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}