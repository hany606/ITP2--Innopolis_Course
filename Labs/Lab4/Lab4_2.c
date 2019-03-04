#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

    //---------------------------2-------------------------------------
    if(argc == 1){
        fprintf(stderr, "Please provide at least one file\n");
        return 1;
    }

    FILE *fptr;
    for(int i = 1; i < argc; i++){
        char *fileName = argv[i];
        fptr = fopen(fileName,"r");
        if(fptr == NULL){
            fprintf(stderr, "No such a file\n");
            return 1;
        }
        char ch;
        while((ch = fgetc(fptr)) != EOF)
            printf("%c",ch);
        printf("\f");
        
        fclose(fptr);

    }

    



    return 0;
}