#include<stdlib.h>
#include<stdio.h>
#include<math.h>

short int Sub_Key_Get(int v, char k){
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

short int permute(short int v){
    char look_up_table[] = {0,1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
    //from 2 as the 1st one doesn't change
    for(int i = 2; i <= 16; i++){
        short int flag = v&(1<<(i-1));
        flag >>= (i-1);
        // printf("FLAG = %d\n",flag);
        //Set the other bit
        if(flag == 1)
            v |= (1<<(look_up_table[i]-1));
        
        //erase the other bit
        else
            v &= !(1<<(look_up_table[i]-1));
    }
}

short int encrypt(int key, short int text){
    short int subkey;
    for(int i = 0; i < 4; i++){
        subkey = Sub_Key_Get(key,i);
        subkey ^= text;
        subkey = SBox(subkey);
        subkey = permute(subkey);
    }
    subkey = permute(subkey);
    subkey ^= Sub_Key_Get(key,4);
    return subkey;
}

short int decrypt(int key, short int text){
    short int subkey = text;
    subkey ^= Sub_Key_Get(key,4);
    subkey = permute(subkey);
    for(int i = 0; i < 4; i++){
        subkey = permute(subkey);
        subkey = SBox(subkey);
        subkey = Sub_Key_Get(key,i);
        subkey ^= text;
    }
    return subkey;
}

int main(){
    // printf("%d",sizeof(short int));//2 Byte = 16 bit
    int val = 0b00111010100101001101011000111111;
    printBinary(Sub_Key_Get(val,1));
    //problem in printing
    //Encryption
    int k = 0b00111010100101001101011000111111;
    short int message = 0b0010011010110111; 
    short int encryptedMessage = 0b1011110011010110;
    printf("Encrypted Message -> Real: %d , Our: %d\n",encryptedMessage,encrypt(k,message));
    return 0;
}