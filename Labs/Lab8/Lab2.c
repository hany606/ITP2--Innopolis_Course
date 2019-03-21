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



//Using malloc ?? to create a cell in the heap and just return the pointer to it 
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
    
    //Weired behaviour
    // List tmp = l->next;
    // l->next = c;
    // c->next = tmp;
}

// • Write a function that looks up a string in a list and returns
// a pointer to the first cell that contains this string (or a null
// pointer if the list does not contain it).
List findString(List l, char *s) {
    struct Cell tmp;
    List ptr = l;
    while(tmp.next != NULL) {
        if(strcmp(tmp.str, s) == 0)
            return ptr;
        tmp = *(tmp.next);
    }
    return NULL;
}

void traverse(List l) {
    List ptr = l;
    struct Cell tmp = *l;
    //Check the first one, Check the last one, if the list is empty???
    while(tmp.next != NULL) {
        printf("%s", tmp.str);
        tmp = *(tmp.next);

    }
    
}


struct HashTable
{
    int mod;
    List* table;
};

int main() {

    //If you are gonna not to use malloc and create the cells in the stack, you will need to create every time you add in the list, you will need to create a new cell with a new name and this is really bad.
    //Root node
    List root = malloc(sizeof(struct Cell));        //Create the root node
    addFirst(stringToCell("1st"),root);

    // struct Cell c = stringToCell("1st");
    // printf("%s\n",c.str);
    // addFirst(c, root);
    // printf("%s",c.str);
    // //Traverse the List
    // printf("\nTraverse the List\n");
    traverse(root);
    // printf("\n-----------\n");

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