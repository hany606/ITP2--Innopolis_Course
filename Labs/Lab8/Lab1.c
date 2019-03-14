#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void raz(void*, int);

int main() {
    int t[10];
    double z;
    raz(t,10*sizeof(int));
    raz(&z, sizeof(z));

    printf("%d\n", t[4]);
    printf("%f",z);
    return 0;
}

void raz(void* ptr, int n) {
    ptr = calloc(n,sizeof(char));
}