#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <threads.h>
#include <pthread.h>

#define THREADS_NUM 2

pthread_mutex_t mutexBuffer; 

int buffer[25];
int count = 0;

void *producer(void *arg) {
    while (1){
        int x = rand() % 100;

        pthread_mutex_lock(&mutexBuffer);
        if(count < 25){
            buffer[count] = x;
            count++;
        }
        pthread_mutex_unlock(&mutexBuffer);
    }
}

void *consumer(void *arg) {
    while (1)
    {
        pthread_mutex_lock(&mutexBuffer);
        if(count > 0){
            int x = buffer[count-1];
            count--;
            printf("Consumed: %d\n", x);
        }
        pthread_mutex_unlock(&mutexBuffer);
    }
}

int main(){
    srand(time(NULL));
    pthread_t th[THREADS_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);

    int i;
    for(i = 0; i < THREADS_NUM; i++){
        if(i%2 == 0){
            if(pthread_create(&th[i], NULL, &producer, NULL) != 0){
                perror("failed pthread_create");
            }
        }else{
            if(pthread_create(&th[i], NULL, &consumer, NULL) != 0){
                perror("failed pthread_create");
            }
        }
    }

    for (i = 0; i < THREADS_NUM; i++)
    {
        if(pthread_join(th[i], NULL) != 0){
            perror("failed pthread_join");
        }
    }

    pthread_mutex_destroy(&mutexBuffer);
    

    return 0;
}