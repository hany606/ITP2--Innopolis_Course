#include<stdlib.h>
#include<string.h>
#include<stdio.h>

const char *errors[7] = {
                            "Number of cities is out of range",
                            "Chosen initial city does not exist",
                            "Chosen destination city does not exist",
                            "Matrix size does not suit to the number of cities", 
                            "The distance between some cities is out of range", 
                            "Initial and destination cities are not connected",
                            "Structure of the input is invalid"
                            };
FILE *input;
FILE *output;

typedef struct flags
{
    int numberCities:1;
    int initialCity:1;
    int destinationCity:1;
    int blankLine:1;
    int matrixSize:1;
    int distancesValid:1;
    int initialDestinationEdgeValid:1;
    int inputStructure:1;
    
}ValidationFlag;


void programBreaker(int i) {
    printf("%s",errors[i-1]);
    fclose(input);
    fclose(output);
    exit(i);
}

int getToken(int f) {
    int tmp = 0;
    int counter = 0;
    int insignificantZeroFlag = 0;
    int negativeValueFlag = 1;
    char c;
    while(1) {
        c = getc(input);

        if((c == ' ' && f == 0 && counter != 0) || (c == '\n' && (f == 1 || f == 3) && counter != 0) || (c == EOF && f == 2 && counter != 0))
            break;
        
        // if it should be EOF but it is new line and there is another input (more size) ,,, if it should new line but it is EOF (less size) if it should be space but it is new line or EOF
        else if((f == 2 && c == '\n' && ((c = getc(input) >= '0') && c <= '9')) || (f == 3 && c == EOF) || (f == 0 && (c == EOF || c == '\n')) )
            programBreaker(4);

        // insignificant zero
        else if(counter == 0 && c == '0') {
            insignificantZeroFlag = 1;
            counter++;
            continue;   // as tmp is already 0
        }
        else if(counter == 0 && c == '-') {
            negativeValueFlag = -1;
            counter++;
            continue;
        }
        else if(!(c >= '0' && c <= '9') || (counter == 0 && c == ' ') || (counter == 0 && c == '\n') || (counter == 0 && c == EOF))
            programBreaker(7);

        if(insignificantZeroFlag == 1)
            programBreaker(7);
        tmp = 10*tmp + (c - '0');
        counter++;
    }
    return tmp*negativeValueFlag;
}




int main() {

    input = fopen("input.txt","r");
    output = fopen("HanyHamedOutput.txt","w");
    char c;
    int numberCities = 0;
    int initialCity = 0;
    int destinationCity = 0;
    int counter = 0;
    ValidationFlag flag;
    


    if(input == NULL) 
        return 1;


    numberCities = getToken(0);
    if(!(numberCities >= 5 && numberCities <= 50)) 
        programBreaker(1);
    flag.numberCities = 1;
    printf("------------\n");

    initialCity = getToken(0);
    if(!(initialCity >= 0 && initialCity <= numberCities-1)) 
        programBreaker(2);
    flag.initialCity = 1;
    printf("------------\n");

    initialCity = getToken(1);
    if(!(initialCity >= 0 && initialCity <= numberCities-1)) 
        programBreaker(2);
    flag.initialCity = 1;
    printf("------------\n");
    
    printf("%d:%d:%d\n",numberCities,initialCity,destinationCity);
    printf("------------\n");

    c = fgetc(input);
    if(c != '\n')
        programBreaker(7);
    
    int i,j,tmp;
    int adjMatrix[numberCities][numberCities];

    for(i = 0; i < numberCities; i++) {
        for(j = 0 ; j < numberCities-1; j++) {

            tmp = getToken(0);
            printf("%d ",tmp);

            if((!(tmp >= 1 && tmp <= 20) && i != j) || i == j && tmp != 0)
                programBreaker(5);
            
            // printf("%d ",tmp);
            adjMatrix[i][j] = tmp;

        }
        if(i == numberCities -1)
            break;

        tmp = getToken(3);
        if((!(tmp >= 1 && tmp <= 20) && i != j) || i == j && tmp != 0)
            programBreaker(5);
        adjMatrix[i][j] = tmp; 
        printf("%d\n",tmp);

    }
    tmp = getToken(2);
        if((!(tmp >= 1 && tmp <= 20) && i != j) || i == j && tmp != 0)
        programBreaker(5);
    adjMatrix[i][j] = tmp; 
    printf("%d\n",tmp);

    return 0;
}