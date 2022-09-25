#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 200
#define CONSUMERS 4

int main(){
    int fdarr[CONSUMERS][2];

    // Create pipes for all consumers
    for (int i = 0; i < CONSUMERS; i++)
    {
        int res = pipe(fdarr[i]);
        if (res == -1)
        {
            printf("Pipe creation failed for consumer %d", i+1);
            return 1;
        }
    }

    // Create child process [1]
    int pid = fork();
    if (pid == -1)
    {
        printf("Fork failed");
        return 1;
    }

    // Child process
    if(pid == 0){
        int consumer_id = 1;
        int pid2 = fork(); // [2]

        // Child process 
        if(pid2 == 0){
            consumer_id = 2;
        }

        int pid3 = fork(); // [3]

        if(pid3 == 0){
            consumer_id = consumer_id + 2;
        }

        char data[BUFFER_SIZE];

        close(fdarr[consumer_id-1][1]);
        while (1)
        {

            int res = read(fdarr[consumer_id-1][0], data, BUFFER_SIZE);
            if (res > 0)
            {
                if(strcmp(data, "EOF") == 0){
                    printf("\nConsumer %d finished\n", consumer_id);
                    break;
                }
                printf("Consumer %d: %s", consumer_id, data);
                // printf("Consumer %d received :\n %s\n", consumer_id, data);
            }
            // return 0;
        }

        close(fdarr[consumer_id-1][0]);
        


    }
    // Parent process
    else{
        // Close all read ends of pipes
        for (int i = 0; i < CONSUMERS; i++)
        {
            close(fdarr[i][0]);
        }

        // Open file
        FILE *fp = fopen("weather.txt", "r");
        if (fp == NULL)
        {
            printf("File open failed");
            return 1;
        }

        // Read file line by line
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
        {
            // Write to all pipes
            for (int i = 0; i < CONSUMERS; i++)
            {
                write(fdarr[i][1], buffer, BUFFER_SIZE);
            }
        }

        // Send something to all pipes to indicate end of file
        for (int i = 0; i < CONSUMERS; i++)
        {
            write(fdarr[i][1], "EOF", BUFFER_SIZE);
        }

        // Close all write ends of pipes
        for (int i = 0; i < CONSUMERS; i++)
        {
            close(fdarr[i][1]);
        }

        // Wait for child process to finish
        wait(NULL);
    }

    return 0;
}