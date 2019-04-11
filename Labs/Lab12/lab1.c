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

char SBox_helper(char v){
    char look_up_table[] = {0xE,0x4,0xD,0x1,0x2,0xF,0xB,0x8,0x3,0xA,0x6,0xC,0x5,0x9,0x0,0x7};
    return look_up_table[v];
}

short int SBox(short int v){
    char res[4];
    short int tmp;

    for(int i = 0; i < 4; i++){
        tmp = v>>(i*4);
        tmp <<= 12;
        tmp >>= 12;
        res[i] = tmp;
        short int tmp2 = SBox_helper(res[i]);
        tmp2 &= __INT16_MAX__;
        tmp2 <<= (i*4);
        v &= tmp2;
    }    
}


int main(){
    // printf("%d",sizeof(short int));//2 Byte = 16 bit
    int val = 0b00111010100101001101011000111111;
    printBinary(Sub_Key_Get(val,1));
    //problem in printing
    return 0;
}