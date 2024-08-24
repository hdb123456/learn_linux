#include <stdio.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/sem.h>

/*ftok



,shmget,shmat,semopen_rsemopen_w,semwait,fgets,sem_post*/



int main()
{   
    key=ftok(".",1);
    semget(key,)
    sem_init();

    sem_wait();
    sem_post();

    sem_destroy();


}
