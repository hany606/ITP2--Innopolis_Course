#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

    //---------------------------3-------------------------------------
    if(argc <= 2){
        fprintf(stderr, "Please provide two files\n");
        return 1;
    }

    FILE *fptr1, *fptr2;
    char *fn1 = argv[1];
    char *fn2 = argv[2];
    fptr1 = fopen(fn1,"r");
    fptr2 = fopen(fn2,"r");
    
    //check the size first it is a better way
    
    char ch1,ch2;
    int flag = 0;
    while( ((ch1 = fgetc(fptr1)) != EOF) || ((ch1 = fgetc(fptr2)) != EOF) ){
        if(ch1 != ch2){
            flag = 1;
        }

        if(flag == 1){
            printf("1st file line: \n%c",ch1);
            while( ((ch1 = fgetc(fptr1)) != EOF) && (ch1 != '\n')) printf("%c",ch1);
            printf("\n2nd file line: \n");
            while( ((ch2 = fgetc(fptr2)) != EOF) && (ch2 != '\n')) printf("%c",ch2);
            break;
        }

    }


    fclose(fptr1);
    fclose(fptr2);
    return 0;
}