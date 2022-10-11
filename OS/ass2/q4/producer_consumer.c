#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_MAX_ELEMENTS 25
#define MIN_RANDOM_NUMBER 1
#define MAX_RANDOM_NUMBER 80

int getRandomNumber()
{
    return rand() % ((MAX_RANDOM_NUMBER + 1) - MIN_RANDOM_NUMBER) + MIN_RANDOM_NUMBER;
}

struct Buffer{
    int data[BUFFER_MAX_ELEMENTS + 1];
    int readCur;  // read cursor
    int writeCur; // write cursor
    int count;    // count
    sem_t *space_available_mutex;
    sem_t *data_available_mutex;
};

// struct ConsumerPIDs
// {
//     int *pids;
//     int count;
// };

void initBuffer(struct Buffer *buffer)
{
    buffer->readCur = -1;
    buffer->writeCur = 0;
    buffer->count = 0;

    // Init mutex
    buffer->space_available_mutex = sem_open("mutex74656", O_CREAT, 0777, 1);
    buffer->data_available_mutex = sem_open("mutex7496", O_CREAT, 0777, 1);
}

void insertData(struct Buffer *buffer, int data)
{
    if (buffer->count == BUFFER_MAX_ELEMENTS)
        return;
    buffer->data[buffer->writeCur] = data;
    buffer->writeCur = (buffer->writeCur + 1) % BUFFER_MAX_ELEMENTS;
    buffer->count++;
    sem_post(buffer->data_available_mutex);
}

int getData(struct Buffer *buffer)
{
    if (buffer->count == 0)
        return -1;
    buffer->readCur = (buffer->readCur + 1) % BUFFER_MAX_ELEMENTS;
    buffer->count--;
    sem_post(buffer->space_available_mutex);
    return buffer->data[buffer->readCur];
}

// void addConsumer(struct ConsumerPIDs *consumerPIDs, int pid)
// {
//     consumerPIDs->pids[consumerPIDs->count] = pid;
//     consumerPIDs->count = consumerPIDs->count + 1;
// }

struct Buffer *buffer;
// struct ConsumerPIDs *consumerPIDs;
int *total;
int *noOfProducersExited;

int main()
{
    srand(time(NULL));
    int noProducers, noConsumers;

    // Init buffer
    buffer = mmap(NULL, sizeof *buffer, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    total = mmap(NULL, sizeof *total, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    noOfProducersExited = mmap(NULL, sizeof *total, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // consumerPIDs = mmap(NULL, sizeof *consumerPIDs, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    initBuffer(buffer);

    *total = 0;
    *noOfProducersExited = 0;
    // consumerPIDs->count = 0;
    // consumerPIDs->pids = (int *)malloc((noConsumers + 1) * sizeof(int));

    // Get number of producers and consumers
    printf("Enter number of producers: ");
    scanf("%d", &noProducers);
    printf("Enter number of consumers: ");
    scanf("%d", &noConsumers);

    if (noProducers == 0 || noConsumers == 0)
    {
        printf("No producers or consumers. Exiting...\n");
        return 0;
    }

    int id;

    // Spin producers 
    for (int i = 0; i < noProducers; i++)
    {
        // Producers
        id = fork();
        if (id == 0)
        {
            // If buffer full wait
            if (buffer->count == BUFFER_MAX_ELEMENTS)
            {
                sem_wait(buffer->space_available_mutex);
            }
            // Generate random no
            time_t t;
            srand((int)time(&t) % getpid());
            int num = getRandomNumber();
            // Insert data
            insertData(buffer, num);
            // Print log
            printf("Producer %d generated no %d\n", getpid(), num);
            *noOfProducersExited = *noOfProducersExited + 1;
            exit(0);
        }
    }

    // Spin consumers

    if (id > 0)
    {
        for (int i = 0; i < noConsumers; i++)
        {
            // Consumers
            id = fork();
            if (id == 0)
            {
                while (1){
                    // if(*noOfProducersExited == noProducers && buffer->count == 0) break;
                    // If no data please wait
                    while (buffer->count == 0)
                    {
                        if (*noOfProducersExited == noProducers)
                            break;
                        // sem_wait(buffer->data_available_mutex);
                        usleep(10000);
                    }
                    // Get dta from buffer
                    int data = getData(buffer);
                    // Add
                    *total = *total + data;
                    // Print log
                    // printf("Consumer %d consumed no %d\n", getpid(), data);
                }
                exit(0);
            }
        }

        for (int i = 0; i < noConsumers; i++)
        {
            wait(NULL);
        }

        while (noProducers < *noOfProducersExited)
        {
            usleep(10000);
        }
        

        printf("Total %d\n", *total);
    }



    return 0;
}
