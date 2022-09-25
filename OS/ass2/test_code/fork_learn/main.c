#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
    // ?
    // int id = fork();
    // printf("hello world from id : %d\n", id);
    // if(id == 0){
    //     printf("Hello from child process\n");
    // }else{
    //     printf("Hello from parent process\n");
    // }

    // ?
    // int id = fork();
    // int n;
    // if(id == 0){
    //     // child process
    //     n = 1;
    // }else{
    //     n = 6;
    // }
    // if(id != 0){
    //     wait(NULL);
    // }
    // int i;
    // for(i = n; i < n+5; i++){
    //     printf("%d ", i);
    //     fflush(stdout);
    // }

    int id = fork();

    printf("Current ID : %d, parent ID : %d\n", getpid(), getppid());

    int res = wait(NULL);
    if(res == -1){
        printf("No child process is waiting\n");
    }
    else{
        printf("%d finished execution\n", res);
    }
    return 0;
}