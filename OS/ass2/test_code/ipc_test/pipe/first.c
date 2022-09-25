#include <unistd.h>
#include <stdio.h>

int main(){
    int fd[2];
    // fd[0] is for reading, fd[1] is for writing
    if(pipe(fd) == -1){
        printf("An error occured while creating the pipe");
        return 1;
    }

    int id = fork();
    if(id == 0){
        close(fd[0]);
        int x;
        printf("Enter a number: ");
        scanf("%d", &x);
        write(fd[1], &x, sizeof(x));
        close(fd[1]);
    }else{
        close(fd[1]);
        int x;
        read(fd[0], &x, sizeof(x));
        printf("The number is: %d\n", x);
        close(fd[0]);
    }

    return 0;
}