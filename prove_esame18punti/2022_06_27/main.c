#include<stdio.h>
#include<stdlib.h>
#include "grafo.h"
#include "ST.h"
typedef enum{false, true} bool;

typedef struct node *link;

//STRUCT for adjacency list
struct node{
    int w;
    link next;
};

// graph wrapper
struct graph{
    int V;  
    link *ladj;     //list of adj, same as matrix
};



GRAPH graph_load (char *filename){
    FILE *fp = fopen(filename, "r");
    int V;
    fscanf(fp, "%d", &V);
    GRAPH G = graph_init(V);        // allocation

    int v, w;
    while (fscanf (fp, "%d %d", &v, &w) == 2){
        insert_node (G->ladj[v], w);            // insert edge (v, w)
        insert_node(G->ladj[w], v);            // insert edge (w, v)
    }

    return G;
}



bool check_domantic_partition (int *partition, GRAPH G, int n_partion, int len){
    if (len != G->V) return false;      // not a valid partition (has to consider all the vertex to be valid)

    int i, j;
    int curr_part;
    for (curr_part = 0; curr_part < n_partion; curr_part++){        // cycling trough the groups
        for (j = 0; j < G->V; j++){             // analising each vertex 
            if (partition[j] != curr_part){         // that is not part of the current group
                bool is_linked_to_part = false;    
                link t;
                for (t = G->ladj[j]; t != NULL && !is_linked_to_part; t = t->next){     // looking for the adjacent elements
                    if (partition[t->w] == curr_part){          // if one of the adjacent belongs to the group we are good
                        is_linked_to_part = true;
                    }
                }
                if (!is_linked_to_part) return false;           // no match found --> partition not valid
            }

        }
    }
    return true;        // everything's okay
}

// FILE FORMAT  (standard partition format)
// 5 10
// 1 this means vertex 0 belongs to first group
// 3  vertex 2 belongs to third group
bool check_proposal(char *filename, GRAPH G){
    FILE *fp = fopen(filename, "r");
    int n_part, len;
    fscanf (fp, "%d ", &n_part, &len);      // reading the cardinality of the partition and number of vertex used 
    int *partition = malloc(len*sizeof(int));       
    int i = 0, group;
    while (fscanf (fp ,"%d", &group) == 1){
        partition[i] = group;           // putting in position i the group i belongs 
        i++;
    }
    bool check = check_domantic_partition(partition, G, n_part, len);

    free(partition);
    return check;
}

// easy ER algorithm to calculate partion
void find_max_partitionER(int *partition, int curr_part, int pos, GRAPH G, int *best_cardinality, int *best_partion){
    int i, j;
    if (pos == G->V){       // base case
        if (check_domantic_partition(partition, G, curr_part, pos)){        // checking if the partion is valid
            if(curr_part > *best_cardinality){      // updating the best cardinality in case
                for (i = 0; i < pos; i++){
                    best_partion[i] = partition[i];     // copying the best solution (not requested)
                    (*best_cardinality) = curr_part;
                }
            }
        }
        return;
    }

    // first recursive call with the same class
    for (i = 0; i < curr_part; i++){
        partition[pos] = i;
        find_max_partitionER(partition, curr_part, pos+1, G, best_cardinality, best_partion); 
    }

    partition[pos] = curr_part;
    // creating the new group
    find_max_partitionER(partition, curr_part+1, pos+1, G, best_cardinality, best_partion);

    return;
}




// SECOND OPTION MORE EFFIECIENT 
// doing partitiong with dispositions 
bool find_max_partion_D(int *partition, GRAPH G, int pos, int partition_class){
    if (pos == G->V){       // base case
        return check_domantic_partition(partition, G, partition_class, pos);        // if it is valid domantic i found the cardinality
    }

    int i;
    for (i = 0; i < partition_class; i++){
        partition[pos] = i;         // assignment 
        if (find_max_partion_D(partition, G, pos+1, partition_class)){
            return true;        // we can return true
        }
    }   
    return false;   // nothing found 
}

void wrapper (GRAPH G){
    int *partition = malloc(G->V*sizeof(int));
    int k;
    for (k = G->V-1; k > 0; k--){       // starting in wiht maximum cardinality and decreasing it
        if (find_max_partion_D(partition, G, 0, k)) break;
    }    

    free(partition);
}




int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}



