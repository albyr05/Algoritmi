#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"
#include <string.h>
#define MAX_S 30

// Simble table wrapper
struct simbletable {
    int N;
    char **m;
};

// Graph wrapper 
struct graph{
    int V;
    int **madj;
    ST tab;
    l *ladj;
};

typedef struct node *link;
// item of the adj list --> contains both of the linked node and the weight 
typedef struct{
    int index;
    int wt;
} Item;

// node of the adj list
struct node {
    Item value;
    link next;
};

// wrapper adj list 
struct ladj{
    link h;
    int N;
};


ST tab_init (int v){
    ST tab = malloc(sizeof(*tab));
    tab->N = 0;
    tab->m = malloc(v*sizeof(char*));
    for (int i = 0; i < v; i++){
        tab->m[i] = malloc(MAX_S*sizeof(char));     
        tab->m[i][0] = '\0';        // intializing everything to empty string
    }
    return tab;
}

l* ladjs_init(int V){
    l *v = malloc(V*sizeof(l));     // allocating the vector
    for (int i = 0; i < V; i++){
        v[i] = malloc(sizeof(*v[i]));       // allocating each list wrapper 
        v[i]->h = NULL; v[i]->N = 0;
    }
    return v;
}

int **matrix_init (int r, int c, int val){
    int **m = malloc(r*sizeof(int*));
    for (int i = 0; i < r; i++){
        m[i] = malloc(c*sizeof(int));
    }
    for (int i = 0; i < r; i++){
        for (int j = 0 ; j < c; j++){
            m[i][j] = val;          // initializing all the matrix to vector 
        }
    }
    return m;
}

// preparing the graph wrapper 
Graph graph_init(int V){
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->madj = matrix_init(V, V, 0);
    G->tab = tab_init(V);
    G->ladj = ladjs_init(V);

    return G;
}

// function used to get index of a node or adding it to the simble table (used while reading the file)
int insert_in_ST(char *label, ST tab){
    int i = 0;
    for (i = 0; i < tab->N; i++){
        if (strcmp(label, tab->m[i]) == 0){
            return i;       // already in the ST
        }
    }
    strcpy(tab->m[tab->N], label);      // adding a new name 
    int ind = tab->N; tab->N++;     
    return ind;     // returning the new index 

}

// inserts the edge in the matrix of adj
void insertE(int id1, int id2, int wt, int** madj){
    madj[id1][id2] = wt;
    madj[id2][id1] = wt;
}


Graph readfile(char *filename){
    FILE *fp;
    int i ;
    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }

    int count = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;        // counting the line 
    }

    rewind(fp);     // going up to the beginning 

    Graph G = graph_init(2*count);          // initializing the graph with the upper bound 

    int id1, id2, wt; 
    char label1[2*MAX_S], label2[MAX_S], label3[2*MAX_S], label4[MAX_S];

    while (fscanf(fp, "%s %s %s %s %d", label1, label2, label3, label4,  &wt) == 5){
        strcat(label1, label2); strcat(label3, label4);     // merging the two strings to get one name
        id1 = insert_in_ST(label1, G->tab);         // retriving the index or getting the new one
        id2 = insert_in_ST(label3, G->tab);
        insertE(id1, id2, wt, G->madj);     // inserting the edge
    }
    G->V = G->tab->N;       // updating the number of vertexes based on the ones actually inserted 

    fclose(fp);
    return G;       // returning the wrapper 
}

// merge sort 
void merge(int * supp_v, ST tab, int l, int m, int r){
    int i = l; int j = m+1, k = 0;
    int *s = malloc((r-l+1)*sizeof(int));
    while(i <= m && j <= r){
        if (strcmp(tab->m[supp_v[i]], tab->m[supp_v[j]]) <= 0){
            s[k++] = supp_v[i++];
        }
        else s[k++] = supp_v[j++];
    }
    while(i <= m) s[k++] = supp_v[i++];
    while(j <= r) s[k++] = supp_v[j++];

    for (i = 0; i < k; i++) supp_v[l+i] = s[i];

    free(s);
}

void mergeS (int *supp_v, ST tab, int l, int r){
    if (l >= r) return;
    int m = (l+r)/2;
    mergeS(supp_v, tab, l, m);
    mergeS (supp_v, tab, m+1, r);
    merge(supp_v, tab, l, m, r);
}


// building an array of indexes, sorting it based on the alfabethic order of the nodes
//read this array in order but retriving the node name based on the index present in pos[i] which has a corresponding index j linked to the ST
void print_in_order(int **madj, ST tab){
    int *supp_v = malloc((tab->N)*sizeof(int));
    
    for (int i = 0; i < tab->N; i++){
        supp_v[i] = i;          // index array to sort 
    }
    mergeS(supp_v, tab, 0, tab->N - 1);  
    
    int *edges = malloc((tab->N)*sizeof(int));      // index array to sort
    for (int i = 0; i < tab->N; i++){
        int k = 0;
        for (int j = 0; j < tab->N; j++){
            if (madj[supp_v[i]][j] != 0) edges[k++] = j;
        }
        
        mergeS(edges, tab, 0, k - 1);   // sorting also the edges 
        
        printf("NODE (%d): %s is linked to:\n", supp_v[i], tab->m[supp_v[i]]);    // printing the original name 
        for (int j = 0; j < k; j++){
            // printing the node linked with the one i have printed ebfore
            printf("     %s   weight[%d]\n",  tab->m[edges[j]], madj[supp_v[i]][edges[j]]);
        }
        
        printf("\n-------------------------------------\n");
    }
    free(supp_v);
    free(edges);
}

void print_order_wrap (Graph G){
    print_in_order(G->madj, G->tab);
}

// creating the new node
link new_node (Item val, link next){
    link x = malloc(sizeof(*x));
    x->value.index = val.index; x->value.wt = val.wt;
    x->next = next;
    return x;
}

// inserting the node in the head;
void insert_node(l adj, Item val){
    adj->N ++;
    if (adj->h == NULL) {
        adj->h = new_node(val, NULL);
        return ;
    }
    adj->h = new_node(val, adj->h);
    return ;
}

// populating the list of adj based on the matrix 
void create_ladj(l* v, int **madj, int V){
    int i, j, k;
    Item val_to_copy;       // preparing the value of the node 
    for (i = 0; i < V; i++){        // cycling on the matrix 
        for (j = 0; j < V; j++){
            if (madj[i][j] != 0 ){          //  there is an edge between i and j
                val_to_copy.index = j; val_to_copy.wt = madj[i][j];     // copying the link destination and the weight 
                insert_node (v[i], val_to_copy);        // passing the i-esim element of the list ;
            }       
        }
    }
    return;
}

void create_ladj_w(Graph G){
    create_ladj(G->ladj, G->madj, G->V);
}
// checking the 2 by 2 adjacency with matrix
bool check_matrix_adj (int id1, int id2, int id3, int ** madj){
    if (madj[id1][id2] != 0 && madj[id1][id3] != 0 && madj[id2][id3] != 0) return true;
    return false;
}

// checking the 2 by 2 adjacency with the adj list 
bool check_l_adj (int id1, int id2, int id3, l*v){
    link x; 
    bool flag1 = false, flag2 = false, flag3 = false;
    for (x = v[id1]->h; x != NULL; x = x->next){
        if (x->value.index == id2) flag1 = true;
    }
    for (x = v[id2]->h; x != NULL; x = x->next){
        if (x->value.index == id3) flag2 = true;
    }
    for (x = v[id1]->h; x != NULL; x = x->next){
        if (x->value.index == id3) flag3 = true;
    }
    return (flag1 && flag2 && flag3);
}

// function used to get index from the ST based on the user input
// couldn't use the first one because it would have created new indexes if wrong inputs
int get_index_ST(char *label, ST tab){
    for (int i = 0; i < tab->N; i++){
        if (strcmp(tab->m[i], label) == 0)
            return i;
    }
    return -1;   // NOT FOUND
}

bool check_three_adj(char *label1, char *label2, char *label3, int ** madj, l* v, ST tab, option o){
    int idx1 = get_index_ST(label1, tab);
    int idx2 = get_index_ST(label2, tab);
    int idx3 = get_index_ST(label3, tab);

    if (idx1 < 0 || idx2 < 0 || idx3 < 0){
        printf("Node not present\n");
        return false;
    }

    if (o == check_matrix)
        return check_matrix_adj(idx1, idx2, idx3, madj);

    if (o == check_ladj)
        return check_l_adj(idx1, idx2, idx3, v);

    return false;
}

bool check_adj_w (char *label1, char *label2, char *label3, Graph G, option o){
    return check_three_adj(label1, label2, label3, G->madj, G->ladj, G->tab, o);
}