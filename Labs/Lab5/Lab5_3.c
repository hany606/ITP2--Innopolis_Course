#include<string.h>
#include<stdlib.h>
#include<stdio.h>

// union birthdate
// {
//     short d;
//     short m;
//     short y;

// };

#define BASE_YEAR 1900


struct{
    unsigned short day :5;
    unsigned short month :4;
    unsigned short year :7;
}birthDateS;



int main() {

    unsigned short birthdate = 0;
    int tmpy,tmpm,tmpd = 0;
    scanf("%d %d %d",&tmpd,&tmpm,&tmpy);
    //birthdate 16 bit (7 bits max(128) for month-1900),(4 bits max(16) for month),(5 bits max(32) for day)
    birthdate = tmpd + (tmpm << 5 ) + ((tmpy - 1900)<<9);
    printf("%hu\n", birthdate);
    tmpy = 0;
    tmpd = 0;
    tmpm = 0;
    tmpy = (birthdate>>9)+1900;
    birthdate = birthdate<<7;
    tmpm = (birthdate>>12);
    birthdate = birthdate<<4;
    tmpd = (birthdate>>11);
    printf("%d %d %d", tmpd, tmpm, tmpy);






    // The code below is not working correctly (Check it)
    // FILE *fptr = fopen("Persons' Birthdate.txt","w");

    // union birthdate ub;

    // scanf("%hu",&ub.d);
    // fputs(ub.d, fptr);
    // fputs("asd",fptr);

    // scanf("%hu",&ub.m);
    // fputs(ub.m,fptr);
    // fputs("\n",fptr);

    // scanf("%hu",&ub.y);
    // fputs(ub.y,fptr);


    // fclose(fptr);
    
    // printf("%d",sizeof(int));        //  4 bytes

    
}