#include "speed.h"
#include <stdio.h>

void speedUp(Speed *s){
    s->val++;
    if(isAuthorizedSpeed(*s) == 0)
        s->val--;
}

void speedDown(Speed *s){
    s->val--;
    if(isAuthorizedSpeed(*s) == 0)
        s->val++;
}
int main() {
    Speed s1;
    s1.val = 1;
    printf("Initial speed %d",start(s1));

    printf("\n##########SPEED UP##########\n");
    while(1){
        int tmp = s1.val;
        speedUp(&s1);
        if(tmp == s1.val)
            break;
        printf("Current Speed: %d\n",s1.val);

    }
    printf("\n##########SPEED Down##########\n");
    while(1){
        int tmp = s1.val;
        speedDown(&s1);
        if(tmp == s1.val)
            break;
        printf("Current Speed: %d\n",s1.val);
        
    }
    
}