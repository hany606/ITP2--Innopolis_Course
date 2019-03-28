#include<stdio.h>

#define BUFFSIZE 10

void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void bublesort(int *a){
    printf("%p\n",&*a);
    printf("%p\n",(&(*a))+1);   //It adds by int* not by the size of the array 
                                //So, you can know the size of the array in the scope of defintion
                                //of the array but not inside the scope of the function
                                //Solution: usse global variable and store the value of the size or
                                //use #define
    
    unsigned long long t1 = (long long) ((&*a)+1);
    unsigned long long t2 = (long long) (&*a);
    
    printf("\nNumber of elements in the array: %llu\n",(t1-t2)/(int)sizeof(int));

    for(int i = 0; i < BUFFSIZE; i++){
        for(int x = i; x < BUFFSIZE; x++){
            if(*(a+i)>*(a+x))
                swap((a+i),(a+x));
        }
    }
}


void swapGeneric(void *a, void *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int cmp(void* a, void* b){
    int ai = (int) *a;
    int bi = (int) *b;
    if(ai > bi)
        return 1;
    return 0;
}

void bublesortGeneric(void *a){
    for(int i = 0; i < BUFFSIZE; i++){
        for(int x = i; x < BUFFSIZE; x++){
            if(cmp((a+i),(a+x)))
                swapGeneric((a+i),(a+x));
        }
    }
}

int main(){
    int a[BUFFSIZE] = {9,8,7,6,5,4,3,2,1,0};

    printf("%p\n",&a);
    printf("%p\n",&a+1);

    unsigned long long t1 = (long long) (&a+1);
    unsigned long long t2 = (long long) (&a);
    
    printf("\nNumber of elements in the array: %llu\n",(t1-t2)/(int)sizeof(int));

    
    bublesort(a);
    printf("\n");

    for(int i = 0; i < BUFFSIZE; i++)
        printf("%d\t", a[i]);
    printf("\n");
}