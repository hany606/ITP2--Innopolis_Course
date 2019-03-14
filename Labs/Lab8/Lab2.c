#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct Cell
{
    char* str;
    int nbr;
    struct Cell *next;
};

typedef struct Cell* List;

//• Write a function that takes as an argument a string and returns a cell that contains this string.
struct Cell stringToCell(char *s) {
    struct Cell c;
    c.str = s;
    c.nbr = 0;
    return c;
}

//• Write a function that takes as arguments a cell and a list and adds a new cell at the head of the list.
void addFirst(struct Cell *c, List l) {
    List tmp = l->next;
    l->next = c;
    c->next = tmp;
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

int main() {
    // • Write a program that reads the standard input and build the
    // list of the different lines with, for each cell, the number of
    // occurrences of the line; then the program displays out each
    // line of the number of its occurrences.



    // • Modify the previous the program in such a way that it can
    // accept the names of several files as arguments; if there is no
    // name of file, then the program reads the standard input.

}