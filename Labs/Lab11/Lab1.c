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
}

int main(){
    pthread_t id1;
    pthread_t id2;
    pthread_t id3;
    pthread_t id4;

    pthread_create(&id1, NULL, thread_func2, NULL);
    pthread_create(&id2, NULL, thread_func2, NULL);
    pthread_create(&id3, NULL, thread_func2, NULL);
    pthread_create(&id4, NULL, thread_func2, NULL);

    printf("Main G = %d\n", g);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    pthread_join(id3, NULL);
    pthread_join(id4, NULL);

    return 0;
}