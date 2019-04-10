#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int Sub_Key_Get(int v, const int k){
    int offset = 2;
    for(int i = 0; i < k; i++)
        offset *= 2;
    if(offset == 2)
        offset = 0;

    // printf("offset:%d\n",offset);
    if(k >= 0 && k<= 4){
        v >>= offset;
        v <<= (16+offset);
        v >>= (16+offset);
        // printf("VAL:%d\n",v); 
    }
    else
    {
        printf("Wrong Key");
        v = -1;
    }

    return v;
    
}

void printBinary(int n){
    char c [16];
    for(int i = 0 ; i < 16; i++)
        c[i] = '1';
    int counter = 0;
    int firstOneBit = 0;
    while (n) {
        if (n & 1)
            c[counter] = '1';
        else
            c[counter] = '0';
          
        counter++;
            
        n >>= 1;
        if(n == -1)
            break;
    }
    counter--;
    for(int i = 15; i >= 0;i--)
        printf("%c",c[i]);
    printf("\n");

}


int main(){
    int val = 0b00111010100101001101011000111111;
    printBinary(Sub_Key_Get(val,1));
    //problem in printing
    return 0;
}