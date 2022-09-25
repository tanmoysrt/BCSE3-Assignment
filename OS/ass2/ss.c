#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/mman.h>
#define BUFF_LEN 25
#define PRODUCER_MIN 50
#define PRODUCER_RANGE 51

typedef struct reqmem{
    int total, start, count;
    unsigned char buffer[BUFF_LEN];
    sem_t full, empty, mutex;
}requiredmemory;

int id, count, total;

void consumer_handler(int sig){
    printf("Consumer %d : consumed %d items which adds up to: %d\n", id+1, count, total);
	fflush(stdout);
	raise(SIGKILL);
}

int main(int argc, char** argv){
    total = 0;
    if (argc != 3){
		printf("%s: Invalid Arguments\nUsage: %s <producer_count> <consumer_count>\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
    }
    int P, C;
	P = atoi(argv[1]);
	C = atoi(argv[2]);
    requiredmemory *shared = mmap(NULL, sizeof(shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared == MAP_FAILED){
        printf("Memory map failed");
        exit(EXIT_FAILURE);
    }
    shared->count = 0;
	shared->start = 0;
	shared->total = 0;
	sem_t *full, *empty, *mutex;
	empty = &(shared->empty);
	full = &(shared->full);
	mutex = &(shared->mutex);
    sem_init(empty, 1, BUFF_LEN);
    sem_init(full, 1, 0);
    sem_init(mutex, 1, 1);
    pid_t producers[P], consumers[C];
    int p, c;
    for (c = 0; c < C; c++){
        pid_t pid = fork();
        if (pid < 0)  {
            goto __fork_error_handler_c;
        }else if (pid != 0) {
            consumers[c] = pid;
        }else {
            id = c;
            goto __consumer;
        }
        
    }
    for (p = 0; p < P; p++){
        pid_t pid = fork();
        if (pid < 0)  {
            goto __fork_error_handler_p;
        }else if (pid != 0) {
            producers[p] = pid;
        }else {
            id = p;
            goto __producer;
        }
    }
    while (p--)
		wait(NULL);
	while (shared->count)
		sleep(1);
	signal(SIGQUIT, SIG_IGN);
	while (c--){
		kill(consumers[c], SIGQUIT);
	}
	sleep(1);
	printf("Total : %d\n", shared->total);
	exit(EXIT_SUCCESS);
    
    __fork_error_handler_c: {
        perror("Unexpected error occurred while forking consumers\n");
	    for (--c; c >= 0; c--)
		    kill(consumers[c], SIGKILL);
	    exit(EXIT_FAILURE);
    }
    __fork_error_handler_p: {
        perror("Unexpected error occurred while forking producers\n");
        for (--c; c >= 0; c--)
		    kill(consumers[c], SIGKILL);
	    for (--p; p >= 0; p--)
		    kill(producers[p], SIGKILL);
	    exit(EXIT_FAILURE);
    }
    __producer: {
        srand(time(NULL));
        count = PRODUCER_MIN + rand() % PRODUCER_RANGE;
        int cnt = count;
	    while (cnt--){
		    unsigned char num = 1 + rand() % 80;
		    sem_wait(empty);
		    sem_wait(mutex);
		    shared->buffer[(shared->start + (shared->count)++) % BUFF_LEN] = num;
		    sem_post(mutex);
		    sem_post(full);
		    total += num;
	    }
	    printf("Producer %d : produced %d items which adds up to %d\n", id+1, count, total);
	    return 0;
    }
    __consumer: {
        signal(SIGQUIT, consumer_handler);
	    count = 0;
	    while (true){
		    unsigned char num = 0;
		    sem_wait(full);
		    sem_wait(mutex);
		    num = shared->buffer[shared->start];
		    shared->start = (shared->start + 1) % BUFF_LEN;
		    shared->count -= 1;
		    shared->total += num;
		    sem_post(mutex);
		    sem_post(empty);
		    count++;
		    total += num;
	    }
	    return 0;
    }
    
}