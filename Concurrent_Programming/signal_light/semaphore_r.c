//file path:/dev/shm/
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

sem_t *sem_r,*sem_w;
void handle(int sig)
{
    if(sig==SIGINT)
    {
        sem_close(sem_r);
        sem_close(sem_w);
        sem_unlink("mysem_r");
        sem_unlink("mysem_w");
        exit(0);
    }
}

void disruptions(){
    struct sigaction act;
    act.sa_handler=handle;
    act.sa_flags =0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT,&act,NULL);
}
void check(sem_t*sem){
    int sem_value=0;
    sem_getvalue(sem, &sem_value);
    printf("sem:%d\n",sem_value);
}

int main (int argv, char ** argc)
{
    key_t key;
    int shmid,sem_value;
    void *shm;
    disruptions();
    key=ftok(".", 1);
    if(key==-1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
   shmid=shmget(key,500,0666|IPC_CREAT);
   if(shmid==-1)
   {
       perror("shmget");
       exit(EXIT_FAILURE);
   }
   shm=shmat(shmid,NULL,0);
   if(shm==(void*)-1)
   {
       perror("shmat");
       exit(EXIT_FAILURE);
   }
    memset(shm, 0, 500); // 500是共享内存区域的大小，根据您的实际情况进行调整
   sem_r= sem_open("mysem_r",O_CREAT|O_RDWR,0666,1);
   sem_w= sem_open("mysem_w",O_CREAT|O_RDWR,0666,0);

    while(1)
    {
        printf("sem_r:");
        check(sem_r);
        printf("sem_w:");
        check(sem_w);

        sem_wait(sem_r);

        printf("sem1_r:");
        check(sem_r);
        printf("sem1_w:");
        check(sem_w);

        printf(">%s",(char*)shm);

        sem_post(sem_w);
        
        printf("sem2_r:");
        check(sem_r);
        printf("sem2_w:");
        check(sem_w);
    }
    return 0;
}