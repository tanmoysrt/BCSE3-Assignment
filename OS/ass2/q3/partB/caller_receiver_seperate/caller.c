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

    // Create fifo
    if (mkfifo(FIFO_NAME, 0777) == -1)
    {
        if (errno != EEXIST)
        {
            printf("An error occured while creating the fifo\n");
            return 1;
        }
    }

    int fd = open(FIFO_NAME, O_WRONLY);
    char inputData[BUFFER_SIZE];

    while (1)
    {
        printf("Enter message: ");
        fgets(inputData, BUFFER_SIZE, stdin);

        write(fd, inputData, BUFFER_SIZE);
        if (strcmp(inputData, "end\n") == 0)
        {
            printf("\nCaller: I am hang up call\n");
            break;
        }
    }

    close(fd);
    return 0;
}