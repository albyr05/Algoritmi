#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



// implementa valid input se te la senti e vai avanti con il menu
typedef enum {false, true} bool;
typedef struct{
    char code[6];
    char name[50];
    char surname[50];
    char date[11];
    char street[50];
    char city[50]; 
    int cap;
} item;

typedef struct n nodo, *link;
struct n{
    item val;
    link next;
};

// reverses the dates and compares them
int cmp_reverse_date (char *date1, char *date2){
    char new_date1[11];
    char new_date2[11];
    int i, j;
    for (i = 6, j = 0; i < strlen(date1); i++, j++){
        new_date1[j] = date1[i];
        new_date2[j] = date2[i];
    }
    new_date1[4] = '/';
    new_date2[4] = '/';
    for (i = 3, j = 5; i < 6; i++, j++){
        new_date1[j] = date1[i];
        new_date2[j] = date2[i];
    }
    for (i = 0, j = 8; j < strlen(date1); i++, j++){
        new_date1[j] = date1[i];
        new_date2[j] = date2[i];
    }
    new_date1[10] = '\0';
    new_date2[10] = '\0';
    return strcmp (new_date1, new_date2);
}


// creating the new node 
link new_node (link next, item v){
    link x = malloc(sizeof(nodo));
    if (x == NULL) return NULL;
    x->val = v;
    x->next = next;
    return x;
}

// function used to insert the node in the right place of the list
link insert_node(item v, link h){
    link x, p;
    if (h == NULL || cmp_reverse_date(v.date, h->val.date) > 0) return new_node(h, v);      // checking if the list is empty or if the new node is the first to insert
    for (x=h->next, p = h; x != NULL && cmp_reverse_date (v.date, (x->val).date) < 0; p = x, x = x->next); // looking for the correct position for the new node
    p->next = new_node(x,v);
    return h;
}

// searching for the node with the input code
void search_element_by_code(link h, char *input_code, item *found_el){
    link x;
    bool flag = false;
    for (x=h; x!=NULL && !flag; x = x->next){
        if (strcmp(x->val.code, input_code) == 0){
            flag = true;
            *found_el = x->val;
        }
    }
    if (!flag){
        printf ("nessun nodo trovato");
    }
}

// extracting an element given a code
link extract_element_by_code(link h, char *input_code, item *found, bool *flag){
    link x = h, p = NULL, t;
    while (x != NULL){
        if (strcmp (input_code, x->val.code) == 0){
            *found = x->val;

            if (p == NULL) h = x->next;     //updating the head if removing the first element

            else p->next = x->next;         // changing pointers to skip the removed el
        
            free(x);
            return h;       // returng the new h
        }
        p = x;
        x = x->next;
    }
    *flag = false;      // case code wasn't found
    return h;
}

// searching the first node in the range
link search_date(link h, link *p, char*date_rng1){
    link x = h;
    while (cmp_reverse_date(date_rng1, x->val.date) < 0){
        *p = x;             // saving also the previous element 
        x = x->next;
    }
    
    return x;
}

link extract_el_by_data(link *f, link *p, item *value, link *head) {
    link next = (*f)->next;     // saving the element after the one i'm extracting
    *value = (*f)->val;             // saving the value i'm esctracting

    if (*p != NULL)
        (*p)->next = next;
    else
        *head = next;    // if i'm extracting the head i need to update it 

    free(*f);           // frees the node removed
    return next;        // returning the next (instead of doing it in a loop because i have no access to f anymore)
}  

// checking the code format 
bool valid_code(char *c){
    int n;
    if (!(c[0]== 'A' && sscanf(c+1, "%d", &n) == 1 && strlen(c) == 5)){
        return false;
    }
    return true;
}

// checking the date format
bool valid_date (char *d){
    int g, m, a;
    if (sscanf (d, "%d/%d/%d", &g, &m, &a) != 3){
        return false;
    } 
    else return true;
}
// validating the input value of the user
bool valid_element(item v, link h){
    for (link x = h; x != NULL; x = x -> next){         // checking if the code is unique in the list
        if (strcmp(v.code, x->val.code) == 0) return false;
    }
    return (valid_code(v.code) || valid_date(v.date));
}

// reading the input file
link read_file(char *filename, link h){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("file not found"); 
        return NULL;
    }
    item v;
    while (fscanf(fp, "%s %s %s %s %s %s %d", v.code, v.name, v.surname, v.date, v.street, v.city, &v.cap) == 7){
        h = insert_node(v, h);
    }
    fclose(fp);
    return h;
}

// printing the list 
void print_list (link h, FILE *fpout){
    link x = h; int i;
    if (fpout == NULL){
        printf("file not found"); exit(1);
    }
    printf("\n");
    for (x = h, i = 1 ; x != NULL; x = x->next, i++){
        fprintf(fpout,"NODO %d: %s %s %s %s %s %s %d\n", i, x->val.code, x->val.name, x->val.surname, x->val.date, x->val.street, x->val.city, x->val.cap);
        
    }
}

// main menu that handles input
link menu(link h, bool *f){
    int choice;
    printf ("\nwhat do you wanna do:\n1. insert a new element in the list\n2. insert multiple elements from a new file\n3. search an element given a code\n4. delete an element given a code\n5. remove elements in between 2 given dates\n6. printing the list on an output file\n");
    scanf("%d", &choice);
    switch (choice){
        case 1: {
            item new_value;         // adding a new value from input
            printf("Insert the new element\n");
            scanf("%s %s %s %s %s %s %d", new_value.code, new_value.name, new_value.surname, new_value.date, new_value.street, new_value.city, &new_value.cap);
            if (valid_element(new_value, h)){
                h = insert_node(new_value, h);
                print_list(h, stdout);
            }
            else printf("invalid format for element insertion\n");
            break;
        }
        case 2:{            // reading nodes from input file
            printf("\nInsert the name of the file: ");
            char filename[256];
            scanf ("%s", filename);
            h = read_file(filename, h);
            print_list(h, stdout);
            break;
        }
        case 3: {           // search element by code
            char code[5];
            printf("Insert a code: ");
            scanf("%s", code);
            if (valid_code(code)){
                item found_el;
                search_element_by_code(h, code, &found_el);
                printf ("\nNodo trovato:\n");
                printf("%s %s %s %s %s %s %d", found_el.code, found_el.name, found_el.surname, found_el.date, found_el.street, found_el.city, found_el.cap);
            }
            else printf("invalid code");
            break; 
        }
        case 4:{            // extracting node by code
            char code[5];
            printf("Insert a code: ");
            scanf("%s", code);
            if (valid_code(code)){
                item extracted_el;      // setting up a value 
                bool flag = true;
                h = extract_element_by_code(h, code, &extracted_el, &flag);
                if (flag){
                    printf("\n%s %s %s %s %s %s %d\n", extracted_el.code, extracted_el.name, extracted_el.surname, extracted_el.date, extracted_el.street, extracted_el.city, extracted_el.cap);
                }
                else printf("No element found with that code");
            }
            else printf("invalid code");
            break;
        }
        case 5:{                // extracting nodes in between two dates
            char date_rng1[11], date_rng2[11];
            printf("Insert the first date: "); 
            scanf("%s", date_rng1);
            printf("Insert the second date: "); 
            scanf("%s", date_rng2);
            if (valid_date(date_rng1) && valid_date(date_rng2)){
                link p = NULL;
                link f = search_date(h, &p, date_rng1);         // retrieving the first node in the range
                item extracted_el;  
                if(f == NULL) printf("No node found in this range");
                else{
                    printf("\nEXCTRACTED NODES\n");
                    for (; f != NULL && cmp_reverse_date(f->val.date, date_rng2) > 0;){
                        f = extract_el_by_data(&f, &p, &extracted_el, &h);          // updating f with the its next node
                        printf("\n%s %s %s %s %s %s %d\n", extracted_el.code, extracted_el.name, extracted_el.surname, extracted_el.date, extracted_el.street, extracted_el.city, extracted_el.cap);
                    }
                }
                print_list(h, stdout);      
            }
            else printf("invalid format for dates");
            break;
        }
        case 6:{                // printing on the output file 
            char filename[256];
            printf("insert the name of the output file: ");
            scanf ("%s", filename);
            FILE *fpout = fopen(filename, "w");
            print_list(h, fpout);
            fclose(fpout);
            break;
        }
        default:{           // wrong input or end of program
            *f = false;
            break;
        }
    }
    return h;
}

// frees the allocated memory for the list
void free_list(link h){
    link x, next;
    for (x=h; x != NULL; x = next){
        next = x -> next;
        free (x);
    }
}


int main(){
    link h = NULL;
    bool f = true;
    while (f) h = menu(h, &f);
    printf("\nProgramma terminato.");
    free_list(h);
    return 0;
}