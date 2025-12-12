#include<stdio.h>
#include<stdlib.h>
#include "title.h"
#include "data_ora.h"
#include "quotazione.h"

#define MAX_S 20
#define S 2
typedef enum{false, true} bool;

int main(int argc, char* argv[]){
    int choice = 0;
    bool flag = true;
    title_list l = list_init();
    printf("\n WELCOME :)");
    while(flag){
        printf("\n what would you like to do: \n1. Load data from input file\n2. Searching a title by name\n3. Searching for a quote in a date, given the title name and date\n4. Searching for the min/max quote of a title in a given date range\n5. Searching for all-time min/max quote of a given title\n6. Balancing the bst\n\n");
        scanf(" %d", &choice);
        switch (choice){
            case 1:{
                char filename[256];
                printf("\nInsert the name of the input file: ");
                scanf(" %s", filename);
                load_list(filename, l);
                printf("\nInput file loaded successfully");
                printlist(l);
                break;
            }
            case 2:{
                char titlename[MAX_S];
                printf("\nInsert the name of the title: ");
                scanf(" %s", titlename);
                if (search_title_by_name(l, titlename) != NULL) printf("\n title found!");
                else printf("\n title not found ");
                break;
            }
            case 3:{
                char titlename[MAX_S];
                date_t datetosearch;
                printf("\nInsert the name of the title: ");
                scanf(" %s", titlename);
                printf("Insert a date (aaaa/mm/dd): ");
                scanf(" %04d/%02d/%02d", &datetosearch.year, &datetosearch.month, &datetosearch.day);
                BST bst = search_title_by_name(l, titlename);
                if(bst != NULL){
                    search_BST(bst, datetosearch);
                }
                break;
            }
            case 4:{
                char titlename[MAX_S];
                date_t date1, date2;
                printf("\nInsert the name of the title: ");
                scanf(" %s", titlename);
                printf("Insert the first date (aaaa/mm/dd): ");
                scanf(" %04d/%02d/%02d", &date1.year, &date1.month, &date1.day);
                printf("Insert the second date (aaaa/mm/dd): ");
                scanf(" %04d/%02d/%02d", &date2.year, &date2.month, &date2.day);
                BST bst = search_title_by_name(l, titlename);
                if(bst != NULL){
                    search_range_BST(bst, date1, date2, range);
                }
                break;
            }
            case 5:{
                char titlename[MAX_S];
                date_t date1, date2;
                printf("\nInsert the name of the title: ");
                scanf(" %s", titlename);
                BST bst = search_title_by_name(l, titlename);
                if(bst != NULL){
                    search_range_BST(bst, date1, date2, all);
                }
                break;
            }
            case 6: {
                char titlename[MAX_S];
                printf("\nInsert the name of the title: ");
                scanf(" %s", titlename);
                BST bst = search_title_by_name(l, titlename);
                balance_bst(bst, S);
                break;
            }
            default: {
                flag = false;
                break;
            }
        }
    }
    printf("\nprogram terminated. goodbye");
    printf("\n");
    return 0;
}