#include<stdio.h>
#include<stdlib.h>

#define rowSize 4
#define colSize 4 

struct r{
    char a;
    int b;
    char c;
};

struct q{
    char a;
    char b;
    int c;

};

int main() {
    int **ptr = (int**) malloc(sizeof(int*) * rowSize);

    // for(int i = 0; i < rowSize; i++)
    //     *ptr = (int*) malloc(sizeof(int) * colSize);

    // for(int i = 0; i < rowSize; i++){
    //     for(int j = 0; j < colSize; j++){
    //         int tmp;
    //         scanf("%d",&tmp);
    //         *(*(ptr+i)+j) = tmp;
    //     }
    // }
    
    struct r r1;
    struct q q1;
    printf("%d\n",sizeof(r1));
    printf("%d",sizeof(q1));


}