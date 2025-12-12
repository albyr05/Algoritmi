#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "quotazione.h"
#include "title.h"
#include "data_ora.h"

typedef enum {false, true} bool;

typedef struct BST_node_s *link;
struct BST_node_s{
    quotazione_t quote;
    link l;
    link r;
    int n;
};

struct BST_wrap{
    link root;
    link z;
};

quotazione_t set_void_quote(){
    quotazione_t quote;
    quote.date.day = -1;
    quote.date.year = -1;
    quote.date.month = -1;
    quote.value = -1.0;  
    return quote;
}

link NEW (quotazione_t val, link l, link r, int N){
    link x = malloc(sizeof (*x));
    if (x == NULL) return NULL; 
    x->quote = val; x->l = l; x->r = r; x->n = N;
    return x;
}

BST bst_init(){
    BST bst = malloc(sizeof *bst);
    if (bst == NULL) return NULL;  
    
    quotazione_t void_quote = set_void_quote();
    bst->z = NEW(void_quote, NULL, NULL, 0);
    if (bst->z == NULL) {  
        free(bst);
        return NULL;
    }
    
    
    bst->root = bst->z;
    
    return bst;
}

int keycmp(date_t a, date_t b){
    char data1[11], data2[11];
    sprintf(data1, "%04d/%02d/%02d", a.year, a.month, a.day);
    sprintf(data2, "%04d/%02d/%02d", b.year, b.month, b.day);
    return strcmp(data1, data2); 
}

link insert(link h, quotazione_t x, link z){
    if (h == z) return NEW(x, z, z, 1);
    if (keycmp(x.date, h->quote.date) >= 0){
        h->r = insert(h->r, x, z);
    }
    else{
        h->l = insert(h->l, x, z);
    }
    (h->n)++;
    return h;
}

void BST_insert (BST bst, quotazione_t x){
    if (bst == NULL) return;  
    bst->root = insert(bst->root, x, bst->z);
}

void bst_searchR (link h, date_t datetosearch, link z){
    if (h == z) {
        printf("Date not found\n");  
        return;
    }
    if (keycmp(h->quote.date, datetosearch) == 0) {
        printf("Daily quote of the selected date: %.2f\n", h->quote.value);  
        return;
    }
    if (keycmp(datetosearch, h->quote.date) > 0){
        bst_searchR(h->r, datetosearch, z);
    }
    else {
        bst_searchR(h->l, datetosearch, z);
    }
}

void search_BST(BST bst, date_t datetosearch){
    if (bst == NULL) return;  
    bst_searchR(bst->root, datetosearch, bst->z);
}

void search_in_range(link h, date_t data1, date_t data2, link z, float *min, float *max, bool *found){
    if (h == z) return;
    
    if (keycmp(h->quote.date, data1) < 0){  
        search_in_range(h->r, data1, data2, z, min, max, found); 
        return;
    }
    if (keycmp(h->quote.date, data2) > 0){  
        search_in_range(h->l, data1, data2, z, min, max, found);  
        return;
    }

    *found = true;
    if (h->quote.value < *min) *min = h->quote.value;
    if (h->quote.value > *max) *max = h->quote.value;

    search_in_range(h->l, data1, data2, z, min, max, found);
    search_in_range(h->r, data1, data2, z, min, max, found);
}

void search_range_BST(BST bst, date_t data1, date_t data2, option o){
    if (bst == NULL) return;  
    
    bool found = false;
    float min = 999999.9; 
    float max = 0;
    date_t min_date = {0, 0, 0}; 
    date_t max_date = {9999, 13, 32};  
    
    if (o == range) {
        search_in_range(bst->root, data1, data2, bst->z, &min, &max, &found);
    }
    else if (o == all) {
        search_in_range(bst->root, min_date, max_date, bst->z, &min, &max, &found);  
    }

    if (found){
        printf("Minimum quote registered in the selected range: %.2f\n", min);
        printf("Maximum quote registered in the selected range: %.2f\n", max);
    }
    else {
        printf("No quote registered in this range\n");  
    }
}

int max(int a, int b){
    return (a > b) ? a : b;  
}

int min(int a, int b){
    return (a < b) ? a : b;  
}

int height(link h, link z){
    if (h == z) return 0;
    return 1 + max(height(h->l, z), height(h->r, z));
}

int min_height (link h, link z){
    if (h == z) return 0;
    return 1 + min(min_height(h->l, z), min_height(h->r, z));
}

link rotL(link h){
    link x = h->r;
    h->r = x->l;
    x->l = h;
    x->n = h->n;
    h->n = 1;
    h->n += (h->l) ? h->l->n : 0;
    h->n += (h->r) ? h->r->n : 0;
    return x;
}

link rotR(link h){
    link x = h->l;
    h->l = x->r;
    x->r = h;
    x->n = h->n;  
    h->n = 1;
    h->n += (h->l) ? h->l->n : 0;
    h->n += (h->r) ? h->r->n : 0;
    return x;
}

link partition(link h, int r){
    if (h == NULL) return NULL;  
    
    int t = (h->l) ? h->l->n : 0;  
    
    if (t > r){
        h->l = partition(h->l, r);
        h = rotR(h);
    }
    if (t < r){
        h->r = partition(h->r, r - t - 1);  
        h = rotL(h);
    }
    return h;
}

link balance (link h, link z){
    int r;
    if (h == z) return z;
    r = (h->n + 1) / 2 - 1;
    h = partition(h, r);
    h->l = balance(h->l, z);
    h->r = balance(h->r, z);
    return h;  
}

void balance_bst(BST bst, int s){
    if (bst == NULL || bst->root == bst->z) return;  
    
    int h = height(bst->root, bst->z);
    int min_h = min_height(bst->root, bst->z);
    
    if (min_h == 0) return;  
    
    if (h / min_h > s){
        bst->root = balance(bst->root, bst->z);
        printf("Tree balanced\n");
    }
    else{
        printf("No balance needed\n");  
    }
}

void free_tree(link tree, link z){
    if (tree == z || tree == NULL) return;
    free_tree(tree->l, z);
    free_tree(tree->r, z);
    free(tree);
}

void free_bst (BST bst){
    if (bst == NULL) return; 
    if (bst->root != bst->z) { 
        free_tree(bst->root, bst->z);
    }
    free(bst->z);
    free(bst);
}