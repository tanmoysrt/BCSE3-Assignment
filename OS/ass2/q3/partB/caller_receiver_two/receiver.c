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
#define FIFO_NAME "telephone_fifo"

int main()
{

    int pid = fork();

    if (pid == -1)
    {
        printf("Fork creation failed");
        return 1;
    }

    // Create fifo
    if (mkfifo(FIFO_NAME, 0777) == -1)
    {
        if (errno != EEXIST)
        {
            printf("An error occured while creating the fifo\n");
            return 1;
        }
    }

    int fdRD = open(FIFO_NAME, O_RDONLY|O_NONBLOCK);
    int fdWR = open(FIFO_NAME, O_WRONLY|O_NONBLOCK);
    char data[BUFFER_SIZE];

    while (1)
    {
        if (read(fdRD, data, BUFFER_SIZE) > 0)
        {
            if (strcmp(data, "end\n") == 0)
            {
                printf("\nReceiver: The caller has hanged up\n");
                break;
            }
            else
            {
                printf("Received: %s\n", data);
                printf("Enter message: ");
                fgets(data, BUFFER_SIZE, stdin);
                // scanf("%s", data);
                write(fdWR, data, BUFFER_SIZE);
            }
        }
    }


    close(fdRD);
    return 0;
}