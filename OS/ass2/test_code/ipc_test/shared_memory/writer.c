#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include<sys/shm.h>  

int main(){
    void* shared_memory;
    int shmid = shmget(1234, 1024, 0777|IPC_CREAT);
    if(shmid == -1){
        printf("An error occured while creating the shared memory\n");
        return 1;
    }
    printf("Key of the shared memory : %d\n", shmid);

    shared_memory = shmat(shmid, NULL, 0);
    printf("The shared memory is attached at %p\n", shared_memory);


    printf("enter some text: ");
    char buffer[1024];
    fgets(buffer, 1024, stdin);
    strcpy(shared_memory, buffer);

    shmdt(shared_memory);

    return 0;
}