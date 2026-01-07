#ifndef UF_H
#define UF_H

typedef struct uf UF;

struct uf {
    int *id;
    int *sz;
    int N;
};

UF UFinit(int N);
int UFfind(UF uf, int p);
void UFunion(UF uf, int p, int q);
void UFfree(UF uf);

#endif