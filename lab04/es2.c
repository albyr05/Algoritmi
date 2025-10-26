#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {false, true} bool;

typedef struct{
    char **song;
    int n_song;
} f;

int read_file (FILE *fp, f ** friends ){
    int n_friends;
    fscanf(fp, "%d", &n_friends);
    f *support = malloc(n_friends*sizeof(f));
    int i = 0;
    while (i < n_friends){
        if (fscanf(fp, " %d", &support[i].n_song) == 1){
            support[i].song = malloc(support[i].n_song*sizeof(char*));
            int j = 0;
            char song_name[255];
            while(j < support[i].n_song){
                fscanf(fp, "%s", song_name);
                support[i].song[j] = malloc(256*sizeof(char));
                strcpy(support[i].song[j], song_name);
                
                j++;
            }  
            i++;
        }
    }
    *friends = support;
    return n_friends;

}

int find_playlist(f *friends, int pos, char **sol, int n, int cnt){
    int i;
    if (pos >= n){
        for (i = 0; i < n; i++){
            printf("%s ", sol[i]);
        }
        printf("\n");
        return cnt+1;
    }
    for (i = 0; i < friends[pos].n_song; i++){
        strcpy(sol[pos], friends[pos].song[i]);
        cnt = find_playlist(friends, pos+1, sol, n, cnt);
    }
    return cnt;
}

int main(){
    FILE *fp = fopen("/Users/albertoreano/Documents/Documenti - MacBook Air di Alberto/vscode/Algoritmi/lab04/es2.txt", "r");
    if (fp == NULL){
        printf("FILE NOT FOUND"); return 1;
    }

    f *friends;
    int n = read_file(fp, &friends);
    char **sol = malloc(n*sizeof(char*));
    for (int i = 0; i < n; i++){
        sol[i] = malloc(255*sizeof(char));
    }
    int cnt = find_playlist(friends, 0, sol, n, 0);
    printf("\n\n%d", cnt);

    return 0;
}