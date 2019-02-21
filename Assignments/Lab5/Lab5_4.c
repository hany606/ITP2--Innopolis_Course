#include<stdlib.h>
#include<stdio.h>
#include<string.h>


//know how to work with char *s as arguments
int stringCmpEnd(char s[], char t[]) {
    int sizeS = 0;
    int sizeT = 0;
    for(int i = 0; s[i] != '\0'; i++) {
        sizeS++;
    }

    for(int i = 0; t[i] != '\0'; i++) {
        sizeT++;
    }

    if(sizeT > sizeS)
        return 0;
    else
    {
        int tmpC = sizeT-1;
        for(int i = sizeS-1; i >= 0; i--) {
            // printf("%c -- %c \n",s[i],t[tmpC]);
            if(s[i] == t[tmpC]) {
                tmpC--;
            }
            else
            {   if(tmpC < 0)
                    return 1;
                return 0;
            }

            
        }
        return 1;
    }
    
}

int main() {
    printf("%d",stringCmpEnd("ASDa","da"));
    
}