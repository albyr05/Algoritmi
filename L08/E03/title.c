#include<stdio.h>
#include<stdlib.h>
#include "title.h"
#include "data_ora.h"
#include "quotazione.h"
#include <string.h>

#define MAX_S 20

typedef struct title_s {
    char name[MAX_S];
    BST quotes_bst;
} title_t;

typedef struct node *link_title;
struct node {
    title_t val;
    link_title next;
};

struct t_list_wrapper {
    link_title head;
    int N;
};

title_list list_init() {
    title_list list = malloc(sizeof(struct t_list_wrapper));
    if (list == NULL) return NULL;
    list->head = NULL;
    list->N = 0;
    return list;
}

link_title new_node(title_t value, link_title next) {
    link_title x = malloc(sizeof(*x));
    if (x == NULL) return NULL;
    x->val = value;
    x->next = next;
    return x;
}

void insert_node(title_list l, title_t value) {
    if (l == NULL) return;
    
    l->N++;
    link_title new = new_node(value, NULL);
    if (new == NULL) return;
    
    link_title x = l->head;
    link_title p = NULL;
    
    if (l->head == NULL || strcmp(value.name, l->head->val.name) < 0) {
        new->next = l->head;
        l->head = new;
        return;
    }
    
    while (x != NULL && strcmp(value.name, x->val.name) >= 0) {
        p = x;
        x = x->next;
    }
    
    new->next = x;
    p->next = new;
}

BST search_title_by_name(title_list l, char *titlename) {
    if (l == NULL || l->head == NULL) return NULL;
    
    link_title x;
    for (x = l->head; x != NULL; x = x->next) {
        if (strcmp(x->val.name, titlename) == 0) {
            return x->val.quotes_bst;
        }
    }
    return NULL;
}

void convert_date(date_t *data_int, char *data) {
    sscanf(data, "%d/%d/%d", &data_int->year, &data_int->month, &data_int->day);
}


void load_list(char *filename, title_list l) {
    if (l == NULL) return;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file: %s\n", filename);
        return;
    }

    int n_titles;
    if (fscanf(fp, "%d", &n_titles) != 1) {
        printf("Errore lettura numero titoli\n");
        fclose(fp);
        return;
    }

    for (int i = 0; i < n_titles; i++) {
        char buffer[MAX_S];
        int n_quotes_per_title;

        if (fscanf(fp, "%s %d", buffer, &n_quotes_per_title) != 2) {
            printf("Errore lettura titolo %d\n", i+1);
            break;
        }

        BST title_bst = bst_init();
        if (title_bst == NULL) {
            printf("Errore allocazione BST per %s\n", buffer);
            continue;
        }

        char current_date[11] = {0};
        int day_price = 0, total_quotes = 0;

        for (int j = 0; j < n_quotes_per_title; j++) {
            char date_line[11];
            int price, qty;
            time_struct time;

            if (fscanf(fp, "%s %d:%d %d %d", date_line, &time.hour, &time.minute, &price, &qty) != 5) {
                printf("Errore lettura quotazione %d del titolo %s\n", j+1, buffer);
                break;
            }

            if (j == 0) {
                strcpy(current_date, date_line);
                day_price = price * qty;
                total_quotes = qty;
            } else {
                if (strcmp(current_date, date_line) == 0) {
                    day_price += price * qty;
                    total_quotes += qty;
                } else {

                    if (total_quotes > 0) {
                        quotazione_t quote;
                        convert_date(&quote.date, current_date);
                        quote.value = (float)day_price / total_quotes;
                        BST_insert(title_bst, quote);
                    }

                    strcpy(current_date, date_line);
                    day_price = price * qty;
                    total_quotes = qty;
                }
            }
        }

        if (total_quotes > 0) {
            quotazione_t quote;
            convert_date(&quote.date, current_date);
            quote.value = (float)day_price / total_quotes;
            BST_insert(title_bst, quote);
        }


        title_t value;
        strcpy(value.name, buffer);
        value.quotes_bst = title_bst;
        insert_node(l, value);
    }

    fclose(fp);
}

void printlist(title_list l) {
    if (l == NULL || l->head == NULL) {
        printf("Lista vuota\n");
        return;
    }
    
    link_title x;
    printf("Titles in the list:\n");
    for (x = l->head; x != NULL; x = x->next) {
        printf("%s\n", x->val.name);
    }
}

void freelist(title_list l) {
    if (l == NULL) return;
    
    link_title x = l->head;
    link_title tmp;
    
    while (x != NULL) {  
        tmp = x->next;
        free_bst(x->val.quotes_bst);
        free(x);
        x = tmp;
    }
    
    free(l);
}