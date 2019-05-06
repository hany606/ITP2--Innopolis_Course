#include<stdio.h>

int fib(int i){
    int cur = 0;
    int counter = 0;
    int prev = 1;
    int tmp;
    if(counter++ >= i) goto DONE;
    FIB:
        tmp = cur;
        cur = cur+prev;
        prev = tmp;
        if(counter++ < i)
            goto FIB;
    DONE:
        return cur;

}

int main(){
    printf("%d\n",fib(6));
    //0 1 2 3 4 5 6 7  ....
    //0 1 1 2 3 5 8 13 ....
}