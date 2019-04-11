#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXSIZE  5

int cookie_jar[MAXSIZE];
int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



//The producer
void *producer_func(void *vararg){
    static int cookies_all_counter = 0;
    srand(1);
    while(1){
        pthread_mutex_lock(&mutex);
        printf("ADD in the jar: Cookies in the jar: %d \n",counter);
        cookie_jar[counter++] = rand();
        pthread_mutex_unlock(&mutex);
        printf("All cookies which are produced: %d\n",++cookies_all_counter);
        sleep(1);
        while(counter >= MAXSIZE){}
    }
}

void *consumer_func(void *vararg){
    time_t t;

    srand(1);
    while(1){
        
        pthread_mutex_lock(&mutex);
        printf("Take from the Jar: ");
        printf("Cookie #%d: %d\n",counter,cookie_jar[--counter]);
        pthread_mutex_unlock(&mutex);
        sleep(2);

        while(counter <= 0){}
    }
}



int main(){

    pthread_t id_producer;
    pthread_t id_consumer;
    pthread_create(&id_producer, NULL, producer_func, NULL);
    pthread_create(&id_consumer, NULL, consumer_func, NULL);
    

    pthread_join(id_producer, NULL);
    pthread_join(id_consumer, NULL);
    


    return 0;
}