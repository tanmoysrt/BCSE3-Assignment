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
#define READ_FIFO_NAME "telephone_fifo_line2"
#define WRITE_FIFO_NAME "telephone_fifo_line1"


int main()
{
    // Create fifo
    if (mkfifo(READ_FIFO_NAME, 0777) == -1 || mkfifo(WRITE_FIFO_NAME, 0777) == -1)
    {
        if (errno != EEXIST)
        {
            printf("An error occured while creating the fifo\n");
            return 1;
        }
    }

    int fdReadFifo = open(READ_FIFO_NAME, O_RDONLY|O_NONBLOCK);
    int fdWriteFifo = open(WRITE_FIFO_NAME, O_WRONLY|O_NONBLOCK);
    int isWrite=1;
    char data[BUFFER_SIZE];

    while (1)
    {
        if(isWrite == 1){
            printf("Enter message: ");
            fgets(data, BUFFER_SIZE, stdin);
            data[strcspn(data, "\n")] = 0;
            fflush(stdin);
            write(fdWriteFifo, data, BUFFER_SIZE);
            isWrite = 0;
        }else{
            if (read(fdReadFifo, data, BUFFER_SIZE) > 0)
            {
                if (strcmp(data, "end\n") == 0)
                {
                    printf("\nReceiver: The caller has hanged up\n");
                    break;
                }
                else
                {
                    printf("Received: %s", data);
                    isWrite = 1;
                }
            }
        }
    }


    close(fdWriteFifo);
    close(fdReadFifo);
    return 0;
}