#include<stdio.h>
#include<stdlib.h>

int main() {
    FILE *fptr = fopen("Persons.txt","r");
    char line[1000];
    while(fgets(line,1000,fptr) != NULL){
        printf("First Name: %s", line);
        fgets(line,1000,fptr);
        printf("Last Name: %s", line);
        fgets(line,1000,fptr);
        printf("Age: %s", line);
        fgets(line,1000,fptr);
        printf("Phone Number: %s", line);
        fgets(line,1000,fptr);
        
    }
    
}