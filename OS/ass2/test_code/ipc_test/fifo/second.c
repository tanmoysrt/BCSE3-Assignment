#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


int main(){
    int fd = open("myfifo1", O_RDONLY);

    int x ;
    while (1)
    {
        int res = read(fd, &x, sizeof(x));
        if(res == -1){
            printf("An error occured while reading to the fifo\n");
            return 2;
        }
        else if(res == 0){
            printf("The fifo has been finsihed\n");
            sleep(3);
        }else{
            printf("x = %d\n", x);
        }

    }
    


    close(fd);

    return 0;
}