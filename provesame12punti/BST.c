#include<stdio.h>
#include<stdlib.h>

typedef enum{false, true} bool;
typedef struct node *link;
typedef struct bst *BST;
struct node {
    int val;
    link l;
    link r;
};

struct bst {
    link r;
    link z;
};


int check_balance(link h, link z){
    if (h == z) return 0;
    
    int size_left = check_balance(h->l, z);
    if (size_left == -1) return -1;

    int size_right = check_balance(h->r, z);
    if (size_right == -1) return -1;

    if (abs(size_left - size_right) > 1) return -1;
    
    return 1 + size_left + size_right;
    
}   

bool BST_is_balanced(BST bst){
    return (count_height(bst->r, bst->z) != -1);
}
int main(int argc, char* argv[]){
    
    printf("\n");
    return 0;
}

