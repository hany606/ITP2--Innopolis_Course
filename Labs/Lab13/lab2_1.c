#include<stdio.h>
// void cmd(short a, short *p){
//     if(p && *p < a) 
//     {
//         goto x;
//     }
    
//     x:
//         *p = a;
// }

int fib(int i){
    int cur = 1;
    int counter = 0;
    int prev = 1;
    int prev2 = 0;
    if(i == 0) goto END;
    if(i == 1) goto END;
    
    FIB:
        cur = prev2+prev;
        prev2 = prev;
        prev = cur;
        counter++;
        if(counter == i)
            goto END;
        goto FIB;
    
    END: 
        return cur;

}
// int main(){
//     short p = 1;

//     cmd(2,&p);
//     printf("%d",p);
// }

int main(){
    printf("%d",fib(4));
}