#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

/* 定义共享内存大小 */
#define SHM_SIZE 1024

/* 定义信号量结构体 */
typedef struct{
    sem_t sem_r; /* 读信号量 */
    sem_t sem_w; /* 写信号量 */
}Semaphores;

/* 全局信号量变量 */
Semaphores sems;
/* 全局共享内存指针 */
void *shm=NULL;
/* 读线程ID */
pthread_t tid1;
/* 退出标志 */
volatile sig_atomic_t should_exit = 0;

/* 处理中断信号的函数 */
void handle(int sig);
/* 读线程函数 */
void *read_thread();

int main (int argc, char ** argv)
{
    key_t key=(key_t)0;
    int shmid=0;

    /* 设置信号处理函数 */
    struct sigaction act;
    act.sa_handler=handle;
    act.sa_flags =0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT,&act,NULL);

    /* 生成共享内存键值 */
    key=ftok(".", 1);
    if(key == -1){
        perror("ftok");
        exit(EXIT_FAILURE);
    }
   /* 获取共享内存ID */
   shmid=shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
   if(shmid == -1){
       perror("shmget");
       exit(EXIT_FAILURE);
   }
   /* 将共享内存附加到进程地址空间 */
   shm = shmat(shmid,NULL,0);
   if(shm == (void*)-1)
   {
       perror("shmat");
       exit(EXIT_FAILURE);
   }
   /* 清空共享内存 */
   memset(shm, 0, SHM_SIZE);
   
   /* 初始化信号量 */
   sem_init(&sems.sem_r, 0, 0);
   sem_init(&sems.sem_w, 0, 1);
   /* 创建读线程 */
   pthread_create(&tid1, NULL, read_thread, NULL);

    /* 主循环，直到退出标志为真 */
    while(!should_exit)
    {   
        /* 等待写信号量 */
        sem_wait(&sems.sem_w);
        printf(">");
        fgets((char*)shm, SHM_SIZE, stdin);
        /* 发送读信号量 */
        sem_post(&sems.sem_r);
        if(should_exit) break;
    }
    /* 销毁信号量 */
    sem_destroy(&sems.sem_r);
    sem_destroy(&sems.sem_w);
    /* 分离共享内存 */
    shmdt(shm);
    /* 删除共享内存 */
    shmctl(shmid, IPC_RMID, NULL);
    /* 等待读线程结束 */
    pthread_join(tid1, NULL);

    return 0;
}
/* 中断信号处理函数 */
void handle(int sig)
{
    if(sig==SIGINT)
    {
      should_exit = 1;  //  退出标志  
    }
}

/* 读线程函数 */
void *read_thread()
{
    while(!should_exit)
    {
        /* 等待读信号量 */
        sem_wait(&sems.sem_r);
        printf(">>%s",(char*)shm);
        /* 发送写信号量 */
        sem_post(&sems.sem_w);
    }
    return NULL;
}
// void check(){
//     int sem_value=0;
//     sem_getvalue(&sems.sem_r, &sem_value);
//     printf("sem_r:%d\n",sem_value);
//     sem_getvalue(&sems.sem_w, &sem_value);
//     printf("sem_w:%d\n",sem_value);
// }