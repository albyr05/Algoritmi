#ifndef QUOTAZIONE_H
#define  QUOTAZIONE_H
#include "data_ora.h"

typedef struct {
    date_t date;
    float value;
}quotazione_t;

typedef struct BST_wrap *BST;

typedef enum {range, all} option;

BST bst_init();
void BST_insert (BST bst, quotazione_t x);
void search_BST(BST bst, date_t datetosearch);
void search_range_BST(BST bst, date_t data1, date_t data2, option o);
void balance_bst(BST bst, int s);
void free_bst (BST bst);
#endif