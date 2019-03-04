#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    FILE *fptr = fopen("Persons.txt","w");

    while(1) {
        char str[250];
        scanf("%s",&str);
        //Todo: Make it endless related with the input not with the word "end"
        //Todo: write EOF in the end of the file
        if(strcmp(str,"end") == 0) {
            // fputs(EOF,fptr);     //Not working
            break;
        }
        fputs(str, fptr);       //try to use fwrite as it is secured and this method is not
        fputs("\n",fptr);
        scanf("%s",&str);
        fputs(str, fptr);
        fputs("\n",fptr);
        scanf("%s",&str);
        fputs(str, fptr);
        fputs("\n",fptr);
        scanf("%s",&str);
        fputs(str, fptr);
        fputs("\n-------------------\n",fptr);
    }

    fclose(fptr);

    
    return 0;
}