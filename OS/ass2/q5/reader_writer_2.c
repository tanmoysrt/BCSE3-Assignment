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

// Reader can read parallaly
// Writer will wait for all read to finish before writing
// While writing, no other reader or writer can read or write


#define MIN_RANDOM_NUMBER 1
#define MAX_RANDOM_NUMBER 5

sem_t *write_mutex;
sem_t *read_mutex;
int *read_counter;
int *write_counter;
sem_t *read_counter_mutex;
sem_t *write_counter_mutex;
int *resource;

int getRandomNumber()
{
    return rand() % ((MAX_RANDOM_NUMBER + 1) - MIN_RANDOM_NUMBER) + MIN_RANDOM_NUMBER;
}

int main()
{
    int no_of_readers;
    int no_of_writers;

    printf("Enter no of readers: ");
    scanf("%d", &no_of_readers);
    printf("Enter no of writers: ");
    scanf("%d", &no_of_writers);


    sem_unlink("mutex34");
    sem_unlink("mute56");
    sem_unlink("mutex341");
    sem_unlink("mute561");
    write_mutex = sem_open("mutex34", O_CREAT, 0777, 1);
    read_mutex = sem_open("mutex341", O_CREAT, 0777, 1);

    read_counter_mutex = sem_open("mute56", O_CREAT, 0777, 1);
    write_counter_mutex = sem_open("mute561", O_CREAT, 0777, 1);

    read_counter = mmap(NULL, sizeof *read_counter, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    write_counter = mmap(NULL, sizeof *read_counter, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    resource = mmap(NULL, sizeof *resource, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    int id;

    id = fork();

    if (id == 0)
    {

        // Start reader processes
        for (int i = 0; i < no_of_readers; i++)
        {
            id = fork();
            if (id == 0)
            {
                time_t t;
                srand((int)time(&t) % getpid());
                // Reader process
                sleep(getRandomNumber());
                // Increment read counter
                sem_wait(read_counter_mutex);
                *read_counter++;
                if (*read_counter == 1)
                    sem_wait(write_mutex);
                sem_post(read_counter_mutex);

                // Read
                printf("Reader %d read %d\n", i + 1, *resource);

                // Decrement read counter
                sem_wait(read_counter_mutex);
                *read_counter--;
                if (*read_counter == 0)
                    sem_post(write_mutex);
                sem_post(read_counter_mutex);
                exit(1);
            }
        }
        // Wait for all childs to finish
        for (int i = 0; i < no_of_readers; i++)
        {
            wait(NULL);
        }
        exit(1);
    }
    else
    {
        for (int i = 0; i < no_of_writers; i++)
        {
            id = fork();
            if (id == 0)
            {
                time_t t;
                srand((int)time(&t) % getpid());
                // Writer process
                sleep(getRandomNumber());
                // Increment write counter
                sem_wait(write_counter_mutex);
                *write_counter++;
                if (*write_counter == 1){
                    sem_wait(read_mutex);
                    sem_wait(write_mutex);
                }
                sem_post(write_counter_mutex);

                // Write
                *resource = getRandomNumber();
                printf("Writer %d wrote %d\n", i + 1, *resource);

                // Decrement write counter
                sem_wait(write_counter_mutex);
                *write_counter--;
                if (*write_counter == 0)
                    sem_post(read_mutex);
                    sem_post(write_mutex);
                sem_post(write_counter_mutex);
                exit(1);
            }
        }

        // Wait for all childs to finish
        for (int i = 0; i < no_of_readers; i++)
        {
            wait(NULL);
        }
        wait(NULL);
        // time_t t;
        // srand((int)time(&t) % getpid());
        // // Start writer process
        // sleep(getRandomNumber());
        // // Wait for all readers to finish
        // sem_wait(write_mutex);

        // // Write
        // *resource = getRandomNumber();
        // printf("Writer wrote %d\n", *resource);

        // // Release write mutex
        // sem_post(write_mutex);
    }

    // Wait for all childs to finish
    // for (int i = 0; i < no_of_readers + 1; i++)
    // {
    //     wait(NULL);
    // }

    return 0;
}