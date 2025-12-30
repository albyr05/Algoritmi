#include<stdio.h>
#include<stdlib.h>
#include "graph.h"


typedef struct list_w *l;
typedef struct node *link;
typedef struct {
    int vertex;
    int wt;
} Item;

typedef struct ed{
    int v;
    int w;
    int wt;
}edge;

struct node {
    Item value;
    link next;
};

struct list_w{
    link h;
    int N;
};

struct graph {
    int V;
    int **madj;
    l *ladj;
};

int **madj_init(int V) {
    int **m = malloc(V*sizeof(int**));
    for (int i = 0; i < V; i++){
        m[i] = malloc(V*sizeof(int));
    }
    for (int i = 0; i < V; i++){
        for (int j = 0; j < V;j++){
            m[i][j] = 0;
        }
    }
    return m;
}

l * ladj_init (int V){
    l * v = malloc(V*sizeof(*v));
    for (int i = 0; i < V; i++){
        v[i] = malloc(sizeof(*v[i]));
        v[i]->h = NULL; v[i]->N = 0;
    }
    return v;
}

Graph graph_init(int V){
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->ladj = ladj_init(V);
    G->madj = madj_init(V);
    return G;
}

link new_node(Item val, link next){
    link x = malloc(sizeof(*x));
    x->next = next; x->value = val;
    return x;
}

void insert_node (l wrap, Item val){
    wrap->N++;
    if (wrap->h == NULL) {
        wrap->h = new_node(val, NULL);
        return;
    }
    wrap->h = new_node(val, wrap->h);
    return;
}


void edge_insert(Graph G, int x, int y, int wt){
    G->madj[x][y] = wt;
    //G->madj[y][x] = wt; // not-oriented;
    Item val1 = {y, wt};
    Item val2 = {x, wt};
    insert_node(G->ladj[x], val1);
    //insert_node(G->ladj[y], val2); // not-oriented
}


Graph read_file(char *filename){
    FILE *fp = fopen(filename, "r");
    int v;
    fscanf (fp, "%d", &v);
    Graph G = graph_init(v);
    int x, y, wt;
    while (fscanf (fp, "%d %d %d", &x, &y, &wt) == 3){
        edge_insert(G, x, y, wt);
    }
    return G;
}

edge edge_create(int v, int w, int wt){
    edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}



// visiting algorithm started
bool pathR(Graph G, int v, int w, int *visited){
    if (v == w) return true;
    printf("searching path...");
    visited[v] = 1;
    for (int t = 0; t < G->V; t++){
        if (G->madj[v][t] != 0){
            if (visited[t] == 0){
                if (pathR(G, t, w, visited)){
                    printf("(%d %d)\n", v, t);
                    return true;
                }
            }
        }
    }
    return false;
}


void graph_path(Graph G, int id1, int id2){
    bool found = false;
    int *visited = calloc(G->V, sizeof(int));
    if (id1 < 0 || id2 < 0) return ;

    found = pathR(G, id1, id2, visited);
    if (!found){printf("no path found to connect %d with %d", id1, id2);}
    free(visited);
    return;
}


void dfsR(Graph G, edge e, int *st, int *pre, int *post, int *time){
    st[e.w] = e.v;
    pre[e.w] =  (*time)++;
    link t;
    for (t = G->ladj[e.w]->h; t != NULL; t = t->next){
        if (pre[t->value.vertex] == -1){
            dfsR(G, edge_create(e.w, t->value.vertex, t->value.wt), st, pre, post, time);
        }
    }
    post[e.w] = (*time)++;

}

void dfs_w(Graph G, int id1){
    printf("\nDFS STARTED FROM VERTEX %d\n", id1);
    int *st = malloc(G->V*sizeof(int));
    int *pre = malloc(G->V*sizeof(int));
    int *post = malloc(G->V*sizeof(int));
    for (int i = 0; i < G->V; i++){
        st[i] = -1; pre[i] = -1; post[i] = -1;
    }
    int time = 0;
    dfsR(G, edge_create(id1, id1, 0), st, pre, post, &time);
    for (int i = 0; i < G->V; i++){
        if (pre[i] == -1){
            printf("STARTTING A NEW DFS FROM VERTEX %d\n", i);
            dfsR(G, edge_create(i, i, 0), st, pre, post, &time);
        }
    }
    printf("Parent vector:\n");
    for (int i = 0; i < G->V; i++){
        printf("%d ", st[i]);
    }
    printf("\nDiscovery vector:\n");
    for (int i = 0; i < G->V; i++){
        printf("%d ", pre[i]);
    }
    printf("\nEnd processing vector:\n");
    for (int i = 0; i < G->V; i++){
        printf("%d ", post[i]);
    }
    
    free(st); free(pre); free(post);
}

void dfscc(Graph G, int v, int idcc, int *cc){
    cc[v] = idcc;
    link t;
    for (t = G->ladj[v]->h; t != NULL; t = t->next){
        if (cc[t->value.vertex] == -1){
            dfscc(G, t->value.vertex, idcc, cc);
        }
    }
    return;
}

void graphcc(Graph G){
    printf ("\nLooking for cc");
    int *cc = malloc(G->V*sizeof(int));
    int i, v;
    for (i = 0; i < G->V; i++){
        cc[i] = -1;
    } 
    int idcc = 0;
    for (v = 0; v < G->V; v++){
        if (cc[v] == -1){
            dfscc(G, v, idcc++, cc);
        }
    }
    printf("\nPrinting the connected components\n");
    for (v = 0; v < G->V; v++){
        printf ("vertex %d belongs to %d connected component\n", v, cc[v]);
    }
    free(cc);
    return;
}


void dfsTS(Graph G, int v, int *ts, int *visited, int *time){
    visited[v] = 0;
    link t;
    for (t = G->ladj[v]->h; t != NULL; t = t->next){
        if (visited[t->value.vertex] == -1){
            dfsTS(G, t->value.vertex, ts, visited, time);
        }
    }
    ts[(*time)++] = v;
}

void dagrts(Graph G){
    printf("\n starting topological order...\n\n");
    int *visited = malloc(G->V*sizeof(int));
    int *ts = malloc(G->V*sizeof(int));
    int v;
    int time = 0;
    for (v = 0; v < G->V; v++){
        ts[v] = -1; visited[v] = -1;
    }
    for (v = 0; v < G->V; v++){
        if (visited[v] == -1){
            dfsTS(G, v, ts, visited, &time);
        }
    }
    printf("DAG NODES IN REVERSE TOPOLOGICAL ORDER\n");
    for (v = 0; v < G->V; v++){
        printf ("%d ", ts[v]);
    }
    free(visited); free(ts);
}

Graph graphreverse(Graph G){
    link t; int v;
    Graph R = graph_init(G->V);
    for (v = 0; v < G->V; v++){
        for (t = G->ladj[v]->h; t != NULL; t = t->next){
            edge_insert(R, t->value.vertex, v, t->value.wt);
        }
    }
    return R;
}

void sccdfs(Graph G, int w, int *scc, int *time0, int time1, int *post){
    link t; 
    scc[w] = time1;
    for (t = G->ladj[w]->h; t != NULL; t = t->next){
        if (scc[t->value.vertex] == -1){
            sccdfs(G, t->value.vertex, scc, time0, time1, post);
        }
    }
    post[(*time0)++] = w;
    return;
}

void graphscc(Graph G){
    int v, time0 = 0, time1 = 0;
    Graph R = graphreverse(G);
    int *sccG = malloc(G->V*sizeof(int));
    int *sccR = malloc(G->V*sizeof(int));
    int *postG = malloc(G->V*sizeof(int));
    int *postR = malloc(G->V*sizeof(int));
    for (v = 0; v < G->V; v++){
        sccG[v] = -1; sccR[v] = -1; postG[v] = -1; postR[v] = -1;
    }
    for (v = 0; v < G->V; v++){
        if (sccR[v] == -1){
            sccdfs(R, v, sccR, &time0, time1, postR);
        }
    }
    time0 = 0; time1 = 0;
    for (v = G->V-1; v >= 0; v--){
        if (sccG[postR[v]] == -1){
            sccdfs(G, postR[v], sccG, &time0, time1++, postG);
        }
    }
    printf("\nSTRONGLY CONNECTED COMPONENTS:\n");
    for (v = 0; v < G->V; v++){
        printf("vertex %d in scc number %d\n", v, sccG[v]);
    }
}


void print_graph(Graph G){
    int i, j;
    for (i = 0; i < G->V; i ++){
        for (j = 0; j < G->V; j++){
            if (G->madj[i][j] != 0){
                printf("%d connected to %d\n", i, j);
            }
        }
    }
}



void free_ladj (l* v, int V){
    link x, tmp;
    for (int i = 0; i < V; i ++){
        for (x = v[i]->h; x != NULL;){
            tmp = x;
            free(x);
            x = tmp;
        }
        free(v[i]);
    }
    free(v);
}
void free_madj(int **m, int V){
    for (int i = 0 ; i < V; i++){
        free(m[i]);
    }
    free(m);
}


void graph_free(Graph G){
    free_ladj(G->ladj, G->V);
    free_madj(G->madj, G->V);
    free(G);
}





