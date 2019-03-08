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
int numberShortestPaths = 0;

typedef struct Node
{
    int index;
    int cost;
    int visited:1;
    int numberBestCostsCities;  //This will indicate also if there this is the source or not that it will be zero
    struct Node *prev[49];
}City;


void programBreaker(int i) {

    fprintf(output,"%s",errors[i-1]);
    fclose(input);
    fclose(output);
    exit(i);
}

int getToken(int f) {
    int tmp = 0;
    int counter = 0;
    int insignificantZeroFlag = 0;
    int negativeValueFlag = 0;
    char c;
    int flagNumbers = 0;
    int flagStar = 0;
    while(1) {
        c = getc(input);

        if((c == ' ' && f == 0 && counter != 0) || (c == '\n' && (f == 1 || f == 3) && counter != 0) || (c == EOF && f == 2 && counter != 0))
            break;
        

        // insignificant zero
        else if(counter == 0 && c == '0') {
            insignificantZeroFlag = 1;
            counter++;
            continue;   // as tmp is already 0
        }
        else if(counter == 0 && c == '-') {
            negativeValueFlag = 1;
            counter++;
            continue;
        }

        else if(c == '*'){
            tmp = -25;
            counter++;
            flagStar = 1;
            continue;
        }
        else if((counter == 0 && c == ' ') || (counter == 0 && c == '\n') || (counter == 0 && c == EOF))
            programBreaker(7);
        
        // if it should new line but it is EOF (less size) if it should be space but it is new line or EOF,,, if it should be EOF but it is new line and there is another input (more size) ,,, 
        else if((f == 3 && c == EOF) || (f == 0 && (c == EOF || c == '\n')) || (f == 3 && c == ' ' && ((c = getc(input) >= '0') && c <= '9')) || (f == 2 && c == '\n' && ((c = getc(input) >= '0') && c <= '9')) ) 
            programBreaker(4);

        // This have been added seperately to check first other symbol or not
        else if(!(c >= '0' && c <= '9'))
            programBreaker(7);
        
        if(insignificantZeroFlag == 1)
            programBreaker(7);
        tmp = 10*tmp + (c - '0');
        flagNumbers = 1;
        counter++;
    }
    if(negativeValueFlag == 1)
        programBreaker(5);

    //To solve the invalid input e.g. 1*
    if(flagNumbers == 1 && flagStar == 1)
        programBreaker(7);  
    return tmp;
}

int rangeDistantValidation(int tmp, int i, int j) {
    if(tmp != -25 && ((!(tmp >= 1 && tmp <= 20) && i != j) || i == j && tmp != 0))
        return 1;
    return 0;
}

void printAllPaths(City *c) {
    if(c != NULL && c->prev == NULL) {
        //print
        //global var
        numberShortestPaths++;
        printf("%d. %d",numberShortestPaths,c->index);
    }
    for(int x = 0; x < c->numberBestCostsCities; x++) {
        printAllPaths(c->prev[x]);
        printf(" -> %d",c->index);
    }
}




int main() {

    input = fopen("input.txt","r");
    output = fopen("HanyHamedOutput.txt","w");
    
    char c;
    int numberCities = 0;
    int initialCity = 0;
    int destinationCity = 0;
    int counter = 0;
    


    if(input == NULL) 
        return 1;


    numberCities = getToken(0);
    if(!(numberCities >= 5 && numberCities <= 50)) 
        programBreaker(1);

    initialCity = getToken(0);
    if(!(initialCity >= 0 && initialCity <= numberCities-1)) 
        programBreaker(2);

    destinationCity = getToken(1);
    if(!(destinationCity >= 0 && destinationCity <= numberCities-1)) 
        programBreaker(2);
    
    c = fgetc(input);
    if(c != '\n')
        programBreaker(7);
    
    int i,j,tmp;
    int adjMatrix[numberCities][numberCities];

    for(i = 0; i < numberCities; i++) {
        for(j = 0 ; j < numberCities-1; j++) {

            tmp = getToken(0);
            if(rangeDistantValidation(tmp,i,j) == 1)
                programBreaker(5);
            if(i == j && tmp != 0)
                programBreaker(7);
            
            adjMatrix[i][j] = tmp;

        }
        if(i == numberCities -1)
            break;

        tmp = getToken(3);
        if(rangeDistantValidation(tmp,i,j) == 1)
            programBreaker(5);
        adjMatrix[i][j] = tmp; 
    }
    tmp = getToken(2);
    if(rangeDistantValidation(tmp,i,j) == 1)
        programBreaker(5);
    adjMatrix[i][j] = tmp; 
    if(i == j && tmp != 0)
        programBreaker(7);

    if(adjMatrix[destinationCity][initialCity] == -25)
        programBreaker(6);


    //Dijkstra
    //Check if the structure values of the vars initialized with zeros
    City cities[numberCities];
    cities[initialCity].cost = 0;
    cities[initialCity].prev[0] = NULL;
    
    for(int i = 0; i < numberCities; i++) {
        if(i != initialCity)
            cities[i].cost = 21;    //more than the max cost
        
        cities[i].numberBestCostsCities = 0;
        cities[i].index = i;
        cities[i].visited = 0;

    }

    for(int i = 0; i < numberCities; i++) {
        City *mnCity;
        int mnCost = 20*numberCities;
        int mnIndexCity = 0;
        //Choose the min cost that for first cycle it will be initial city
        for(int x = 0; x <numberCities; x++) {
            if(mnCost >= cities[x].cost && cities[x].visited == 0) {
                mnCity = &(cities[x]);
                mnIndexCity = x;
                mnCost = cities[x].cost;
            }
            
        }

        for(int j = 0; j < numberCities; j++) {
            //If it is connected to it or not
            if(j != mnIndexCity && adjMatrix[j][mnIndexCity] != -25 && cities[j].visited == 0) {
                int tmp = (mnCity->cost)+adjMatrix[j][mnIndexCity] ;
                if(tmp < cities[j].cost){
                    cities[j].cost = tmp;
                    if(cities[j].numberBestCostsCities > 0) {
                        for(int x = 0; x < cities[j].numberBestCostsCities; x++) {
                            cities[j].prev[x] = NULL;
                        }
                    }
                    cities[j].numberBestCostsCities = 1;
                    cities[j].prev[0] = mnCity;   
                }
                else if(tmp == cities[j].cost) {
                    cities[j].prev[cities[j].numberBestCostsCities] = mnCity;
                    cities[j].numberBestCostsCities++;
                }
                
            }
        }
        mnCity->visited = 1;
    }

    printf("The shortest path is %d\nThe number of shortest paths is 1:\n",cities[destinationCity].cost);
    //Then come back again to this line and edit it again
    printAllPaths(cities[destinationCity].prev[0]);
    
    // printf("%d", (*cities[destinationCity].prev[cities[destinationCity].numberBestCostsCities-1]).cost);





    return 0;
}