#include<stdio.h>
#include<stdlib.h>

/**
 * Important Links:
 * https://cdecl.org/       [Abounegm]
 * https://d3s.mff.cuni.cz/legacy/~holub/c_features.html    [Abounegm]
 * https://www.wikiwand.com/en/Digraphs_and_trigraphs
 * https://medium.com/datadriveninvestor/compilation-process-db17c3b58e62
 * https://medium.com/@connorbrereton/what-does-gcc-main-c-do-bc96824a60a4
 * https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work
 * https://en.cppreference.com/w/c/language/operator_precedence
 * 
 * 
 * */

/**
 * Preprocessor: #define, #include, #ifdef, trigraphs(synonoms for things e.g. ??> is converted to } ) preprocessor directives
 * Compilation: gives an assembly code, you can put flags for optimization for the process
 * Assembler:    take the assembly code and converted to object code
 * Linker: take the object files and the libraries and converted to an executable file
 * */

/*
-S get assembly code
-c get object file from c file
-o followed with name of the executable file (get executable file)

gcc without any flag to get executable file

get object code from assembly code use <as>

objdump -d to convert from object code to assembly code (to make it readable)

without anything executable file
read-increment-execute

f4 in 2's complement representations means that the number is negative as f 1111

the counter is by bytes

byte+       the second column is the full equivelant instruction with relative address and the other current address which is the next address
byte+
..
..
with -Og option is the least optimization and make a simple readable assembly file

-O1 is the optimization parameter of the compiler
O0
Og
O1
O2
O3
...

gcc -S -O1 name.c
*/

/**
 * Threads:
 *  are like a new path/flow for a part of the program 
 * Threads terminate by explicitly calling pthread_exit, by letting the function return, or by a call to the function exit which will terminate the process including any threads.
 * 
 * Thread Synchronization:
    The threads library provides three synchronization mechanisms:

    mutexes - Mutual exclusion lock: Block access to variables by other threads. This enforces exclusive access by a thread to a variable or set of variables.
    joins - Make a thread wait till others are complete (terminated).
    condition variables - data type pthread_cond_t

    Mutexes:
    Mutexes are used to prevent data inconsistencies due to race conditions. A race condition often occurs when two or more threads need to perform operations on the same memory area, but the results of computations depends on the order in which these operations are performed. Mutexes are used for serializing shared resources. Anytime a global resource is accessed by more than one thread the resource should have a Mutex associated with it. One can apply a mutex to protect a segment of memory ("critical region") from other threads. Mutexes can be applied only to threads in a single process and do not work between processes as do semaphores. 

 * */

    /**
     * Makefiles & Header Files
     * define new token -> name = val for example cc = gcc
     * use it $(token_name)
     * tag: dependencies
     *      process  $(cc) -std=c11 main.c
    **/



// #define, macros, prototypes, declarations, typedef, functions, return types, prarameters, structures,
// Unions, bit field...

//# and ## don't evaluate or replace it by preprocessing the argument just it put it as it is
//It will evaluate the arguments if there is no sharps
#define toString(a) #a
#define concatenate(a,b) a##b

#define paste(left , right) left ## right


#define print(val) printf("%d\n",val);
#define print2(val) printf("%d\n",val)



//typedef
//pointer to a function that returns int and has 3 arguments
// note that the typedef name is indeed func_name
typedef int (*func_name)(void *, int, int);

//make new name for array of 10 charachters
typedef char string[10];




//Structures: size, padding


struct Final
{
    int i;
};

 

//#ifdef, #ifndef,...

//copy structures and pointers content

int main(){

    printf("%s\n",toString(asdasd));
    printf("concatenate(123,546)\n");
    printf("%d\n",concatenate(123, 42));

    print(5);
    print(5)
    print2(6);
    string s = "asd";
    printf("%s\n",s);
    printf("%s\n",toString(paste(a,b)));
    //if, switch, for, while, do-while,
    for(int i = 0; i < 2; i++)
        if(1)
            if(1)
                if(1)
                    printf("TRUE\n");

    struct Final f;
    struct Final *fpt = &f;
    (*fpt).i = 1;   // => fpt->i = 1; 

    int arr[] = {1,4,5,8};
    int *ptr = arr;
    int tmp  = *ptr++;
    //    int tmp  = *++ptr;
    printf("%d\n",tmp);
    printf("%d\n",arr[0]);
    tmp = *ptr;
    printf("%d\n",tmp);

    //increment the pointer as go after the last element in the array
    printf("%d\n",&arr+1);
    printf("%d\n",*((int*)(&arr)+1));
    printf("%d\n",*((int*)(&arr+2)));   //access other memory
    printf("%d\n",(int)'1'+2);
    // printf("%d\n",arr++);
    //Input & Output
    //Using Stack to evaluate expressions --> It is being evaluated from right to left


    //Files IO
    //freopen -> redirect std IO to file
    //fp = freopen("file.txt", "w+", stdout);
    //After a call to freopen(), it associates STDOUT to file file.txt, so whatever we write at STDOUT that goes inside file.txt. So, the file file.txt will have the following content.

    //Arrays

    //pointers

    //pointer to array, array of pointers, function return pointer , function return array, takes array
    
    //malloc, calloc, realloc, free....

    /**
     * Lecture Notes:
     * When you malloc or calloc a block of memory you actually get more than 
     * your allocation from the system
     * Assume you allocate 10 bytes of memory 
     * The system will return 14 bytes to malloc
     * Malloc will return a pointer to the 5 th Byte
     * These 4 pervious 4 bytes store:
        * The amount of bytes taken (14)
        * A pointer to some other free memory
        * Some flags 
    * Free take an address and clean the memory but not delete the pointer
    * it is better to put it to NULL
    * 
    * Protip!
     The allocation is the object
     The cast and the assigned are pointers
     The malloc sizeof should have ONE LESS *
     int *A =
    (int *)malloc(sizeof(int));
     char **A =
     thing *******A =
    (char **)malloc(sizeof(char *));
    (thing *******)malloc(sizeof(thing ******));
    */

    //Alcoate array of arrays and free
    int **A = (int **)malloc(4*sizeof(int *)); //allocate the array of arrays
    for (int i=0;i<4;i++)
        A[i] = (int *)malloc(3*sizeof(int)); //allocate the arrays
        
    for (int i=0;i<4;i++) //free the inner arrays
        free(A[i]);
    free(A); //free the outer array
    /**/


}