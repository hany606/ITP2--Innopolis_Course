#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv){

    //---------------------------1-------------------------------------
    FILE *fptr;
    fptr = fopen("./file1.txt","r");
    /*
        -There are two pointers points to the std input (command line)
        -When you make pipe for the output stdout change to the pipe file but stderr always points to the std command line
    */
    if(fptr == NULL){
        fprintf(stderr, "No such a file\n");
        return 1;
    }

    char ch;
    while((ch = fgetc(fptr)) != EOF){
        printf("%c",ch);
    }

    fclose(fptr);


    return 0;
}