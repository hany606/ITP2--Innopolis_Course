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

    int tmp = 1;
    if(tmp==1)
        for(int i = 0; i < 50; i++)
            printf("%d\n", i);


}