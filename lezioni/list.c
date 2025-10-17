#include <stdio.h>
#include <stdlib.h>

typedef struct n *link, nodo;
struct n {
    int val;
    link next;
};

link new_node (int v, link h);
link insertHead(link h, int v);

int main(){
    link h = NULL;
    h = insertHead(h, 2);
    h = insertHead(h, 10);
    h = insertHead(h, 3);

    return 0;
}

link new_node (int v, link next){
    link x = malloc(sizeof(*x));
    if (x == NULL){
       return NULL;
    }
    else {
        x->val = v;
        x-> next = next;
    }
    return x;
}

link insertHead(link h, int v){
    h = new_node(v, h);
    return h;
}
