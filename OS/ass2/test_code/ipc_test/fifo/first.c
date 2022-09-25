#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


int main(){
    if(mkfifo("myfifo1", 0777) == -1){
        if(errno != EEXIST){
            printf("An error occured while creating the fifo\n");
            return 1;
        }
    }

    int fd = open("myfifo1", O_WRONLY);

    int arr[] = {1,2,3,4,5,6,7,8,9,10};

    if(write(fd, arr, sizeof(arr)) == -1){
        printf("An error occured while writing to the fifo\n");
        return 2;
    }

    close(fd);

    return 0;
}