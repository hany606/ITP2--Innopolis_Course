//Build it in two header file and see if I include the file twice of linked list

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define BUFFERSIZE 15


//Singly Linked List
struct Cell
{
    char* str;
    int nbr;    //number of occurrences
    struct Cell *next;
};

typedef struct Cell* List;



//• Write a function that takes as an argument a string and returns a cell that contains this string.
struct Cell stringToCell(char *s) {
    struct Cell c;
    c.str = s;
    c.nbr = 1;
    return c;
}

List createRoot(){
    List r = malloc(sizeof(struct Cell));
    r->next = NULL;
    r->str  = "Root";
    return r; 
}


//• Write a function that takes as arguments a cell and a list and adds a new cell at the head of the list.
void addFirst(struct Cell c, List l) {
    struct Cell *tmp = malloc(sizeof(struct Cell));
    tmp->str = c.str;
    tmp->next = l->next;
    l->next = tmp;
}

// • Write a function that looks up a string in a list and returns
List findString(List l, char *s) {
    if(l == NULL)
        return NULL;
    List tmp = l;
    do{
        if(strcmp(tmp->str, s) == 0)
            return tmp;
        tmp = tmp->next;
    }while(tmp != NULL);
    return NULL;
}

void printCell(struct Cell* c) {
    printf("%s",c->str);
}

void traverse(List l) {
    if(l == NULL)
        return;
    List tmp = l;
    printf("\n");
    do{
        printf("%s\t", tmp->str);
        tmp = tmp->next;
    }while(tmp != NULL);
    printf("\n");
    
}



struct HashTable
{
    int mod;
    List* table;
};

int main() {

    //If you are gonna not to use malloc and create the cells in the stack, you will need to create every time you add in the list, you will need to create a new cell with a new name and this is really bad.
    //Root node
    List root = createRoot();        //Create the root node
    addFirst(stringToCell("1st"),root);
    traverse(root);
    printCell(findString(root,"1st"));
    printf("\n");

    printf("\n-----------\n");

    // • Write a program that reads the standard input and build the
    // list of the different lines with, for each cell, the number of
    // occurrences of the line; then the program displays out each
    // line of the number of its occurrences.
    // char in[BUFFERSIZE];
    // while(fgets(in,BUFFERSIZE,stdin) != NULL) {
    //     printf("%s",in);
    // }
    // //EOF == Ctrl+z
    // printf("\nEND of File\n");
    
    // • Modify the previous the program in such a way that it can
    // accept the names of several files as arguments; if there is no
    // name of file, then the program reads the standard input.

}

