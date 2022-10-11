#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    int canwrite = 1;
    char msg[200];
    while (true)
    {
        if (canwrite)
        {

            printf("Enter message: ");
            fgets(msg, 200, stdin);
            msg[strcspn(msg, "\n")] = 0;
            fflush(stdin);
            int fd = open("myfifo", O_WRONLY);
            if (fd == -1)
            {
                printf("Error opening fifo\n");
                exit(1);
            }

            write(fd, msg, sizeof(msg));
            close(fd);
            canwrite = 0;
        }
        else
        {
            int fd = open("myfifo", O_RDONLY);
            if (fd == -1)
            {
                printf("Error opening fifo\n");
                exit(1);
            }
            read(fd, msg, sizeof(msg));
            close(fd);
            printf("Receiver : %s\n", msg);
            canwrite = 1;
        }

        fflush(stdin);
        fflush(stdout);
    }

    return 0;
}