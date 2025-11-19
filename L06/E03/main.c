#include<stdio.h>
#include<stdlib.h>
#include "inv.h"
#include "pg.h"
typedef enum{false, true} bool;
typedef enum{load_pg, load_items, insert_pg, remove_pg, update_equip, print_pg}menu;

int main(int argc, char* argv[]){
    int choice;
    list_pg_w *l = malloc(sizeof(list_pg_w));
    inventory_t *inv = malloc(sizeof(inventory_t));
    bool flag = true;
    list_init(l);
    inventory_init(inv);
    
    while(flag){
        printf("Welcome, what do you wanna do: \n0. Load pg list\n1. Load items\n2. Insert a new pg\n3. Remove a pg\n 4. Update equip\n 5. Print pg details\n");
        scanf(" %d", &choice);
        switch (choice){
        
        case load_pg:{
            char filename[250];
            printf("Insert the name of file: ");
            scanf(" %s", filename);
            pg_list_load(l, filename);
            break;
        }
        case load_items:{
            char filename[250];
            printf("Insert the name of file: ");
            scanf(" %s", filename);
            load_inventory(inv, filename);
            break;
        }
        case insert_pg:{
            insert_pg_from_user(l);
            break;
        }
        case remove_pg:{
            char code[10];
            printf("Insert the code of the pg you want to remove: ");
            scanf(" %s", code);
            pg_list_remove(l, code);
            break;
        }
        case update_equip:{
            char code[10];
            printf("Insert the code of the pg you want to update the equip of: ");
            scanf(" %s", code);
            pg_t *choosen_pg = search_pg_in_list(l, code);
            pg_update_equip(choosen_pg, inv);
            break;
        }
        case print_pg:{
            char code[10];
            printf("Insert the code of the pg you to see the details of: ");
            scanf(" %s", code);
            pg_t *choosen_pg = search_pg_in_list(l, code);
            if(choosen_pg == NULL) printf("pg not present in the list");
            else print_pg_details(choosen_pg);
            break;
        }
        default:
            printf("invalid choice");
            flag = false;
            break;
        }
    }
    printf("\n");
    return 0;
}