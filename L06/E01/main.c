#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
typedef struct {
    int start;
    int end;
    int d;
}activity;

int compare(activity a, activity b){
    if (a.start <= b.start){
        return 1;
    }
    return 0;
}

void merge (int l, int m, int r, activity *v, int N){
    int i = l, j = m+1, k = 0;
    activity * supp= malloc((r-l+1)*sizeof(activity));
    while(i <= m && j <= r){
        if (compare(v[i], v[j]) > 0){
            supp[k++] = v[i++];
        }
        else supp[k++] = v[j++];
    }
    while (i <= m) supp[k++] = v[i++];
    while (j <= r) supp[k++] = v[j++];
    for (i = 0; i < k; i++) v[l+i] = supp[i];
    free(supp);
    return;
}

void mergeSort(int l, int r, activity *v, int N){
    if(l >= r) return;
    int m = (l+r) /2;
    mergeSort(l, m, v, N);
    mergeSort(m+1,r, v, N);
    merge(l, m, r, v, N);
}

int readfile(activity **vect){
    FILE *fp = fopen("activity.txt", "r");
    if (fp == NULL){
        printf("Errore nell'apertura del file");
        exit(-1);
    }
    int N, i;
    fscanf(fp, "%d", &N);
    activity *v = malloc(N*sizeof(activity));       // allocating the base struct vector
    for (i = 0; i < N; i++){
        fscanf(fp, "%d %d", &v[i].start, &v[i].end);
        v[i].d = v[i].end - v[i].start;
    }
    *vect = v;
    fclose(fp);
    return N;
}

bool check_compatibility(activity a, activity b){
    if (a.start <b.end && b.start < a.end) return false;
    return true; 
}

void printsol(activity *v, int N, int *pos, int *last){
    int max = 0, i, index_sol;
    for (i = 0; i < N; i++){
        if (last[i] > max){
            max = last[i]; index_sol = i;
        }
    }
    printf("MAX LASTING %d\n", max);
    for (i = index_sol; i != -1; i = pos[i]){
        printf("(%d, %d) ", v[i].start, v[i].end);
    }
}

void pd(activity *v, int N){
    int i, j;
    int *pos = malloc(N*sizeof(int));
    int *last = malloc(N*sizeof(int));
    for (i = 0; i < N; i++){
        //pos[i] = -1; last[i] = v[i].d;
        int max_d = 0, bj = -1;
        for (j = 0; j < i; j++){
            if (check_compatibility(v[i], v[j])){
                if (last[j] > max_d){
                    bj = j;
                    max_d = last[j];
                }
            }
            last[i] = v[i].d + max_d;
            pos[i] = bj;
        }
    }
    printsol(v, N, pos, last);
    free(pos); free(last);
}


int main(int argc, char* argv[]){
    activity *v;
    int N = readfile(&v);
    mergeSort(0, N, v, N);
    pd(v, N);
    printf("\n");
    free(v);
    return 0;
}