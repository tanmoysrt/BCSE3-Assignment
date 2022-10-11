#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
int main()
{
    char *shared_memory;
    char buff[100];

    int shmid = shmget((key_t)1234, 1024, 0666);
    shared_memory = shmat(shmid, NULL, 0); // process attached to shared memory segment
    printf("Process attached at %p\n", shared_memory);
    printf("Data read from shared memory is : %s\n", (char *)shared_memory);
    shmdt(shared_memory); // detach from shared memory
    shmctl(shmid, IPC_RMID, 0); // destroy the shared memory
}