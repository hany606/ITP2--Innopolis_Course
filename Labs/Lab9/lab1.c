//To get what happening in the preprocessor (gcc -E)
#include<sys/queue.h>           //BSD libc in c for Linked List, queues,...

#include<stdio.h>

//Macro with for loops
#define myloop for(int i = 0; i < 3; i++)

//Question 6
#define square(x) x*x
#define square2(x) (x*x)

//Question 8
int X = 5;                  //This will not work as the preprocessor part before compilation and #if depend on the preprocessor definition
// #define X 3              // If this activated it will run the first one
//the value of X is related to the preprocessor part as #define not variable
//The result of this expression related to the preprocessor expressions not the variables
#if X == 3                      
    #define Y 3
#else
    #define Y 5
#endif






#define PASTE(x,y) x##y
#define PASTE2(x,y) PASTE(x,y)
#define f(x) char* x=#x
#define g(x) x


int main(){
    printf("\n--------------Q1--------------\n");
    myloop  printf("%d",i);
    printf("\n--------------Q6--------------\n");
    printf("%d\t",36/square(6));
    printf("%d\t",36/square2(6));
    printf("\n--------------Q8--------------\n");
    printf("%d",Y);
    
    //Arguments are expanded unless there is # they are just put (I don't understand good)
    //It will evaluate the arguments if there is no sharps
    f(g(a));            //char* a = "g(a)";

    int PASTE2(PASTE2(PASTE2(a,b),c),d);    //int abcd;
    // int PASTE(PASTE(PASTE(a,b),c),d);       //Compilation Error

    printf("%s",a);
}