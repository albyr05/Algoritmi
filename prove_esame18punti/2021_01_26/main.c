#include<stdio.h>
#include<stdlib.h>
#include "struct.h"
#include "ST.h"

typedef enum{false, true} bool;

typedef struct {
    char name[20];
    int depth;
    int treasure;
    int gold;
}room;

typedef struct node *link;

struct node{
    int w;
    int trap;
    link next;
};


struct graph{
    int n_rooms;
    ST st;
    room *rooms;
    link *ladj;
};

struct path {
    int N_rooms;
    int *r_path;    // salvo gli id delle stanze nel percorso
    int total_wealth;
};

link new_node(int dest, bool trap, link next){
    link x = malloc(sizeof(*x));
    x->trap = trap;
    x->w = dest;
    x->next = next;
    return x;
}

void insert_node(link head, int dest, bool trap){
    if (head == NULL) {
        head = new_node(dest, trap, NULL);
        return;
    }
    head = new_node(dest, trap, head);
    return;
}

GRAPH graph_load(char *filename){
    FILE *fp = fopen(filename, "r");
    int n_rooms;
    fscanf (fp, "%d", &n_rooms);
    GRAPH G = graph_init(n_rooms);

    int i, depth, treasure, gold ;
    char name[20];
    for (i = 0; i < n_rooms; i++){
        fscanf(fp, "%s %d %d %d", name, &depth, &treasure, &gold);
        STinsert(G->st, name, i);
        G->rooms[i] = (room){name, depth, treasure, gold};
    }

    char room_a, room_b; bool trap;

    while (fscanf(fp, "%s %s %d", room_a, room_b, &trap) == 3){
        int id_a = STsearch(G->st, room_a);
        int id_b = STsearch(G->st, room_b);
        insert_node (G->ladj[id_a], id_b, trap);
        insert_node (G->ladj[id_b], id_a, trap);
    }
    fclose(fp);
    return G;
}


PATH graph_path_load(GRAPH G, FILE *fp){
    int rooms_in_path;
    fscanf(fp, "%d", &rooms_in_path);
    PATH p = path_init(rooms_in_path);
    char name[20];
    for (int i = 0; i < 0; i++){
        fscanf (fp, "%s", name);
        int id = STsearch(G->st, name);
        p->r_path[i] = id; 
    }
    return p;
}

bool is_start_end (room a){
    return (a.depth == 0 || a.depth == 1 || a.depth == 2);
}

bool graph_path_check(GRAPH G, PATH p, int M, int PF){
    int original_PF = PF;

    if (!(is_start_end(G->rooms[p->r_path[0]]) && is_start_end(G->rooms[p->r_path[p->N_rooms-1]])) )return false;


    int *visited = calloc (G->n_rooms, sizeof(int));
    int best_treasure = 0;

    if (p->N_rooms-1 > M) return false;


    int i; 
    for (i = 0; i < p->N_rooms-1; i++){
        bool linked = false;
        link t;
        for (t = G->ladj[p->r_path[i]]; t != NULL && !linked ; t = t->next){
            if (t->w == p->r_path[i+1]){
                linked = true;
            }
        }
        if (linked){
            if (t->trap == 1) PF --;
            if (visited[p->r_path[i]] == 0) p->total_wealth += G->rooms[p->r_path[i]].gold;
            if (G->rooms[p->r_path[i]].treasure > best_treasure) best_treasure = G->rooms[p->r_path[i]].treasure;
            visited[i] = 1;

            if (PF == 0){
                if (G->rooms[p->r_path[i]].depth > 2) return false;
                if (G->rooms[p->r_path[i]].depth == 2){
                    p->total_wealth /= 2;
                }
                if (G->rooms[p->r_path[i]].depth == 1){
                    p->total_wealth *= (float) 2/3;
                }   
            }
        }

    }

    return true;
}

void find_best_path (int id, GRAPH G, int M, int PF, int *visited,  PATH p,  int pos, int *best_wealth){
    if (visited[id] == 0) p->total_wealth += G->rooms[id].gold;

    if (p->total_wealth > *best_wealth) *best_wealth = p->total_wealth;
    if (PF == 0){
        if (G->rooms[id].depth > 2) return;
        if (G->rooms[id].depth == 2) {
            p->total_wealth *= (float) 2/3;
        }
        if (G->rooms[id].depth == 1){
            p->total_wealth /= 2;
        }
        return ;
    }

    visited[id] = 1;
    if (M <= 0) return;
    
    link t;
    for (t = G->ladj[id]; t != NULL; t->next){
        p->r_path[pos] = t->w;
        int prev_point = PF;
        if (t->trap == 1) PF --;
        find_best_path(t->w, G, M--, PF, visited, p, pos+1, best_wealth);
    }

    visited[id] = 0;


    
}




int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}