#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>


void handle_sigstop(int sig) {
    printf("Stop not allowed");
}

void handle_sigcont(int sig) {
    printf("I am continuing");
}

int main(){

    // struct sigaction sa;
    // sa.sa_handler = &handle_sigstop;
    // sa.sa_flags = SA_RESTART;
    // sigaction(SIGTSTP, &sa, NULL);

    // signal(SIGTSTP, &handle_sigstop);
    signal(SIGCONT, &handle_sigcont);

    int pid = fork();

    if(pid == 0){
        while (1)
        {
            printf("I'm alive\n");
            usleep(90000);
        }
    }else{
        sleep(1);
        printf("Stopping child\n");
        kill(pid, SIGSTOP);
        sleep(1);
        printf("Starting child\n");
        kill(pid, SIGCONT);
        sleep(1);
        printf("Killing child\n");
        kill(pid, SIGKILL);
        wait(NULL);
    }

    return 0;
}