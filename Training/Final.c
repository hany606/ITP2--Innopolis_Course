#include<stdio.h>
#include<stdlib.h>

/**
 * Important Links:
 * https://cdecl.org/       [Abounegm]
 * https://d3s.mff.cuni.cz/legacy/~holub/c_features.html    [Abounegm]
 * https://www.wikiwand.com/en/Digraphs_and_trigraphs
 * https://medium.com/datadriveninvestor/compilation-process-db17c3b58e62
 * https://medium.com/@connorbrereton/what-does-gcc-main-c-do-bc96824a60a4
 * 
 * 
 * */

/**
 * Preprocessor: #define, #include, #ifdef, trigraphs(synonoms for things e.g. ??> is converted to } ) preprocessor directives
 * Compilation: gives an assembly code, you can put flags for optimization for the process
 * Assembler:    take the assembly code and converted to object code
 * Linker: take the object files and the libraries and converted to an executable file
 * */

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



// #define, macros, prototypes, declarations, typedef, functions, return types, prarameters, structures,
// Unions, bit field...



//Structures: size, padding


int main(){

    //if, switch, for, while, do-while, 
    
    //Input & Output
    //Using Stack to evaluate expressions --> It is being evaluated from right to left


    //Files IO


    //Arrays

    //pointers
    
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
    for (int i=0;i<n;i++)
        A[i] = (int *)malloc(3*sizeof(int)); //allocate the arrays
        
    for (i=0;i<n;i++) //free the inner arrays
        free(A[i]);
    free(A); //free the outer array
    /*

    //Makefiles

    //Header files
}