#include<stdio.h>

int main()
{

    if(5 == 5)
        if(1 == 2)
            printf("51");
        else if(2 == 2)
            printf("52");
        //tricky wrong indentation
        printf("out if scope");


}