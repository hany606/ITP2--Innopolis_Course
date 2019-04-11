#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int g = 0;

//So, static variables will be shared with the all threads but not globally with the whole system it has its own scope
//It cannot be called outside the function

void *thread_func(void *vararg){
    int i = 0;
    for(i = 0 ; i < 11; i++){
        printf("T%d\n",i);
        sleep(1);
    }
}

void *thread_func2(void *vararg){
    int l = 0;
    static int s;
    l++; l++;
    g++; g++;
    s++; s++;
    printf("S = %d, G = %d, L = %d\n", s,g,l);
    while(1){}
}

int main(){
    const int  n =10000;
    pthread_t id[n];
    int i = 0;
    for(i = 0; i < n; i++){
        pthread_create(&id[i], NULL, thread_func2, NULL);
    }
    printf("Main G = %d\n", g);

    for(i= 0; i < n; i++){
        pthread_join(id[i], NULL);
    }


    return 0;
}