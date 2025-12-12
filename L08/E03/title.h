#ifndef TITLE_H
#define TITLE_H
#include "quotazione.h"
#include "data_ora.h"
typedef struct title_s *title;
typedef struct t_list_wrapper *title_list;

title_list list_init();
void load_list (char *filename, title_list l);
BST search_title_by_name(title_list l, char *titlename);
void freelist (title_list l);
void printlist (title_list l);

#endif