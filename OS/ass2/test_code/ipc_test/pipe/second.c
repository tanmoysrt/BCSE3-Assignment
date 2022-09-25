#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int arr[] = {1,2,3,4,1,2,7};
    int arrSize = sizeof(arr)/sizeof(int);
    int start, end;
    int fd[2];
    if(pipe(fd) == -1){
        printf("An error occured while creating the pipe");
        return 1;
    }

    int id = fork();
    if(id == -1){
        return 2;
    }

    if(id == 0){
        start = 0;
        end = arrSize/2;
    }else{
        start = arrSize/2;
        end = arrSize;
    }

    int sum = 0;
    for(int i = start; i < end; i++){
        sum += arr[i];
    }
    printf("Partial sum : %d\n", sum);

    if(id == 0){
        close(fd[0]);
        write(fd[1], &sum, sizeof(sum));
        close(fd[1]);
    }else{
        close(fd[1]);
        int sum2;
        read(fd[0], &sum2, sizeof(sum2));
        printf("Total sum : %d\n", sum + sum2);
        wait(NULL);
        close(fd[0]);
    }
}