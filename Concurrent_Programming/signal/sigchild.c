#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void handle (int sig)
{
    wait(NULL);
    printf("Get sig =%d\n,sig");
}


int main(){
    pid_t pid;
    struct sigaction act;
    act.sa_handler=handle;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);

    pid=fork();

    if(pid>0)
    {
        sigaction(SIGCHLD,&act,NULL);
        while(1)
        {
            printf("this is peiqi\n");
            sleep(1);
        }

    }
    else if(pid==0)
    {
        sleep(5);
        exit(0);
    }
}