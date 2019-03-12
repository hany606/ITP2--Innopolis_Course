#include<stdlib.h>
#include<stdio.h>

/**
 * Introduction to Programming 2 (ITP2)
 * Innopolis University, Sprinng 2019
 * Assignment 1
 * Author Hany Hamed
 */

#define MINCityCost 1
#define MAXCityCost 20
#define MINNumberCities 5
#define MAXNumberCities 50

// This is an array of strings for the errors messages
const char *errors[7] = {
                            "Number of cities is out of range",
                            "Chosen initial city does not exist",
                            "Chosen destination city does not exist",
                            "Matrix size does not suit to the number of cities", 
                            "The distance between some cities is out of range", 
                            "Initial and destination cities are not connected",
                            "Structure of the input is invalid"
                            };

//Global Pointers for File I/O
FILE *input;
FILE *output;

int i,j;                //Global variables to store the index where the reading process is on, to pass it to the validation of distance between cities
int initialCity = 0;    //Global variable for the index of the Initial City 
int destinationCity = 0;//Global variable for the index of the Destination City

//Stucture that describe each city in the graph in the Dijkstra Algorithm
typedef struct Node
{
    int index;                  //Store the index of the city
    int cost;                   //Store the cost that results from Dijkstra Algorithm
    unsigned int visited:1;     //Flag to indicate if the city visited or not
    int numberBestCostsCities;  //Indicate how many city have the same best cost from this city
    struct Node *prev[49];      //Store the cities that have the same best cost from this city
}City;


/*
 * Function:  programBreaker 
 * --------------------
 *  Output the specific error according to the parameter i and close files and the program
 *  i: the number of the error, [one-indexed]
 */
void programBreaker(int i) {

    fprintf(output,"%s",errors[i-1]);   //Print the specific Error
    //Close the input and the output files
    fclose(input);                      
    fclose(output);
    // exit(i);                            //Exit the program
    exit(0);            //In order not to give run-time error in automatic grader
}

/*
 * Function:  rangeDistantValidation 
 * --------------------
 *  Validate if the number in the argument is correct to our input constrains or not and if it is in the diagonal and not equal to zero
 *  tmp: indicate the cost from city i to city j (the Global variables)
 *  returns: 1 if it is valid, otherwise 0
 */
int rangeDistantValidation(int tmp) {
    if( (!(tmp >= MINCityCost && tmp <= MAXCityCost) && i != j) 
        || (i == j && tmp != 0))
        return 1;
    return 0;
}


/*
 * Function:  getToken 
 * --------------------
 * Get a token which is number of cities, initial city, destination city and costs 
 * and validate if they have the correct format or not depending on the flag
 * f: Flag to describe the correct position of the read token
 *    it can be 0: Read Number of Cities/Initial City + space ' '
 *              1: Read Destination City + End of the line '\n' 
 *              2: Read the last cost in the matrix + EOF
 *              3: Read the last cost in the each line + End of the line '\n
 *              4: Read the cost + space ' '
 * returns: the number which describe the specific token 
 */
int getToken(int f) {
    int tmp = 0;                    //Store the temporary value of the input until we read all the token successfully
    int counter = 0;                //Store how many character we successfully read
    int insignificantZeroFlag = 0;  //Flag to indicate if there is an insignificant Zero or not to raise error
    int negativeValueFlag = 0;      //Flag to indicate if there is '-'
    char c;                         //variable to store the read character from the file
    int numbersFlag = 0;            //Flag to indicate if the input for the token has a number
    int starFlag = 0;               //Flag to indicate if there is '*'
    //Loop until there is error break the code or it read and return successfully
    while(1) {
        c = getc(input);


        //This to validate the same input format we need
        if((c == ' ' && (f == 0|| f == 4) && counter != 0)      //If it reads a token in the middle followed by space and the input is not an empty space
        || (c == '\n' && (f == 1 || f == 3) && counter != 0)    //If it reads a token in the End of the row followed by '\n' and the input is not an empty space
        || (c == EOF && f == 2 && counter != 0))                //If it reads the last token followed by EOF and the input is not the empty space
            break;
        

        // Insignificant Zero validation
        else if(counter == 0 && c == '0') {
            insignificantZeroFlag = 1;
            counter++;                  //Increase the counter of characters we read
            continue;                   //Go to the next iteration to read the next character as if it was number, it will be error, if it was space or \n depending on the flag it will be just zero token
        }

        // Minus sign validation
        else if(counter == 0 && c == '-') {
            negativeValueFlag = 1;
            counter++;
            continue;
        }

        // Star sign validation
        else if(c == '*'){
            tmp = MAXCityCost+1;       //Store in the tmp the highest cost we have to return if all pther validations correct
            counter++;
            starFlag = 1;
            continue;
        }

        // If the input was only space, new line or EOF that's mean the structure is invalid
        else if((counter == 0 && c == ' ') 
            || (counter == 0 && c == '\n') 
            || (counter == 0 && c == EOF))
            programBreaker(7);              //Structure of the input is invalid
        
        // Matrix size validations
        else if((f == 3 && c == EOF)                                            //Input: number + EOF -> But It should (number + '\n')
            || ((f == 0|| f == 4) && (c == EOF || c == '\n'))                   //Input: number + EOF or \n -> But It should (number + ' ')
            || (f == 3 && c == ' ' && ((c = getc(input) >= '0') && c <= '9'))   //Input: number + ' ' + number-> But It should (number + '\n')
            || (f == 2 && c == '\n' && ((c = getc(input) >= '0') && c <= '9'))) //Input: number + '\n'+ number-> But It should (number + EOF)
            programBreaker(4);                                                  //Matrix size does not suit to the number of cities


        //If it passed from other validations and still other symbol
        else if(!(c >= '0' && c <= '9'))
            programBreaker(7);              //Structure of the input is invalid

        
        //If the flag of Insignificant zero is raised and there is another valid input after it not equal to 0
        if(insignificantZeroFlag == 1 && c != '0')
            programBreaker(7);              //Structure of the input is invalid

        tmp = 10*tmp + (c - '0');           //Add the integer input to the currecnt tmp variable
        numbersFlag = 1;                    //raise the flag of integer input
        counter++;                          //Increment the number of character that we read successfully
    }
    
    //To solve the invalid input (number followed by star) e.g. 1*
    if(numbersFlag == 1 && starFlag == 1)
        programBreaker(7);              //Structure of the input is invalid
    
    //To solve the invalid input (- followed by star) e.g. -*, *-
    if(starFlag == 1 && negativeValueFlag == 1)
        programBreaker(7);              //Structure of the input is invalid
    
    // To returun the max Cost +1 (Unreachable cities)
    if(starFlag == 1)
        return MAXCityCost+1;
    

    // To evaluate the negative numbers
    if(negativeValueFlag == 1)
        tmp *= -1;

    //Check the boundries of the tokens
    if(f >= 2 && rangeDistantValidation(tmp) == 1)
        programBreaker(5);              //The distance between some cities is out of range

    return tmp;
}


/*
 * Function:  countAllPaths 
 * --------------------
 * Count the number of possible shortest path to city c
 * c: the pointer to the destination city
 * returns: the number of shortest path
 */
int countAllPaths(City *c) {
    int tmp = 0;    //Store the number of shortest paths
    // If the function reaches to the greatest parent the initialCity it will return path
    if(c->numberBestCostsCities == 0) 
        return 1;

    //iterate over the possible shortest paths from the current city    
    for(int x = 0; x < c->numberBestCostsCities; x++) 
        tmp += countAllPaths(c->prev[x]);
    
    return tmp;
}



//Global variables related to the printAllPaths function
int shortestPathNumber = 0;     //To Store the current number of path we are printing it
int pathIndex = 0;              //Will store the available index in the array of path to put the city index
/*
 * Function:  printAllPaths 
 * --------------------
 * Print all the shortest path in the specific format
 * c: the pointer to the destination City
 * path: is array that will store the current path to print 
 */
void printAllPaths(City *c,int *path) {
    // If the function reaches to the greatest parent the initialCity it will return path
    if(c->numberBestCostsCities == 0) {
        shortestPathNumber++;           //Increment the number of shortest paths we have
        fprintf(output,"%d. %d",shortestPathNumber,c->index);   //Print the initialCity
        // Print the path in the opposite way from the storing
        for(int i = pathIndex-1; i >= 0; i--)
            fprintf(output," -> %d",path[i]);  //Print the city index
        
        fprintf(output,"\n");
        return;
    }
    // Iterate over the possible solutions for the shortest paths from the current city
    for(int x = 0; x < c->numberBestCostsCities; x++) {
        path[pathIndex] = c->index;             //Store the node in the path
        pathIndex++;
        printAllPaths(c->prev[x],path);          //Recursive call to explore the shortest path for the prev city
        pathIndex--;
    }

}


int main() {

    //Set the names of the files and the modes for them
    input = fopen("input.txt","r");
    output = fopen("HanyHamedOutput.txt","w");

    int numberCities = 0;   //Variable to store the number of the cities 

    //Check if the input file's name is correct
    if(input == NULL) 
        return 1;

    //Read the number of cities and check the boundries
    numberCities = getToken(0);
    if(!(numberCities >= MINNumberCities && numberCities <= MAXNumberCities)) 
        programBreaker(1);  //Number of cities is out of range


    //Read the Initial City and check the boundries
    initialCity = getToken(0);
    if(!(initialCity >= 0 && initialCity <= numberCities-1)) 
        programBreaker(2);  //Chosen initial city does not exist


    //Read the Destination City and check the boundries
    destinationCity = getToken(1);
    if(!(destinationCity >= 0 && destinationCity <= numberCities-1)) 
        programBreaker(3);  //Chosen destination city does not exist

    //Read the empty line
    char c = fgetc(input);
    //If it not an empty line
    if(c != '\n')
        programBreaker(7);  //Structure of the input is invalid


    int tmp;                                    //temporary variable to store the token 
    int adjMatrix[numberCities][numberCities];  //Adjecency matrix to represent the graph

    for(i = 0; i < numberCities; i++) {
        for(j = 0 ; j < numberCities-1; j++) {

            tmp = getToken(4);  //Read cost + space

            //Check if the cost from the city to itself not equal to zero as it can be *
            if(i == j && tmp != 0)
                programBreaker(7);      //Structure of the input is invalid
            
            adjMatrix[i][j] = tmp;      //Store the value after validations

        }
        //If it is the last element in the last row, break to read it with validation of EOF
        if(i == numberCities - 1)
            break;

        tmp = getToken(3);  //Read cost + '\n'
        adjMatrix[i][j] = tmp; 
    }
    tmp = getToken(2);      //Read cost + EOF
    //Check if the cost from the city to itself not equal to zero
    if(i == j && tmp != 0)
        programBreaker(7);
    adjMatrix[i][j] = tmp; 

    //Dijkstra Algorithm
    City cities[numberCities];          //List to store the cities cost and visting status from the Algorithm
    cities[initialCity].cost = 0;       //Initialize the initial city cost
    
    for(i = 0; i < numberCities; i++) {
        //If it is not the initial city put the maximum cost for the longest path that can be
        if(i != initialCity)
            cities[i].cost = (MAXCityCost+1)*numberCities;
        
        //Initialize the array
        cities[i].numberBestCostsCities = 0;
        cities[i].index = i;
        cities[i].visited = 0;
        cities[i].prev[0] = NULL;
    }

    //Iterate over the list of cities
    for(i = 0; i < numberCities; i++) {

        int mnCost = 21*numberCities;   //Store the value of the value of the minimum cost, in oredr to choose the minimum one
        int mnIndexCity = 0;            //Store the index of the minimum city (needed) index

        //Choose the city that has the minimum cost and is not visited yet
        for(int x = 0; x <numberCities; x++) {
            if(mnCost >= cities[x].cost && cities[x].visited == 0) {
                mnIndexCity = x;
                mnCost = cities[x].cost;
            }
        }



        for(j = 0; j < numberCities; j++) {
            //Check if the jth city is connected to the minimum city or not and if it is reachable from it and not visited
            if(j != mnIndexCity && adjMatrix[j][mnIndexCity] <= MAXCityCost && cities[j].visited == 0) {
                int tmp = cities[mnIndexCity].cost + adjMatrix[j][mnIndexCity];     //Add the cost of the edge and the cost of the source city
                
                //If the tmp value is better, this will be the shortest path for the jth city
                if(tmp < cities[j].cost){
                    cities[j].cost = tmp;   //Set the new updated cost
                    
                    //Erase the old list of shortest paths from other nodes
                    if(cities[j].numberBestCostsCities > 0)
                        for(int x = 0; x < cities[j].numberBestCostsCities; x++)
                            cities[j].prev[x] = NULL;       //Make the pointer point to NULL (nothing)

                    cities[j].numberBestCostsCities = 1;
                    cities[j].prev[0] = &cities[mnIndexCity];   //Put the previouse city as the best parent for this city now   
                }
                //If we have equal path, add it to the list
                else if(tmp == cities[j].cost) {
                    cities[j].prev[cities[j].numberBestCostsCities] = &cities[mnIndexCity];
                    cities[j].numberBestCostsCities++;
                }   
            }
        }
        cities[mnIndexCity].visited = 1;    //Mark the city as visited


    }
    //Array that will store the path
    int path[numberCities];
    //Initialize the path by -1 which mean that the value is not representing an index for a city
    for(int i = 0; i < numberCities; i++)
        path[i] = -1;

    // If the destination city is not reachable
    if(cities[destinationCity].cost == (MAXCityCost+1)*numberCities)
        programBreaker(6);  //Initial and destination cities are not connected

    int shortestPathsCounter = countAllPaths(&cities[destinationCity]);     //Get the number of shortest paths

    fprintf(output,"The shortest path is %d.\nThe number of shortest paths is %d:\n",cities[destinationCity].cost,shortestPathsCounter);
    
    printAllPaths(&cities[destinationCity],path);

    fclose(input);
    fclose(output);
    

    return 0;
}