#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

void handle(int sig)
{
    if(sig==SIGINT)
    printf("I catch the sigint\n");
    else if(sig==SIGALRM)
    {
        printf("alarm(1)\n");
      //  alarm(1);
    }
}
/*
struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int,siginfo_t*,void)
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)(void);
}*/
//sigaction(int signum,const struct sigaction *act,struct sigaction* oldact)
int main (){

    struct sigaction act;
    act.sa_handler=handle;
    act.sa_flags =0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM,&act,NULL);
    
//    alarm(1);
//    sigaction(SIGALRM,&act,NULL);

struct itimerval timevalue;
timevalue.it_interval.tv_sec=1;
timevalue.it_interval.tv_usec=0;
timevalue.it_value.tv_sec=1;
timevalue.it_value.tv_usec=0;
setitimer(ITIMER_REAL,&timevalue,NULL);

//sigaction(SIGALRM,&act,NULL);

while(1)
{
 //   sleep(1);
}

}
