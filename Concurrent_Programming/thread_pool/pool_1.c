#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

typedef struct queue
{
	void *arg;
	void *(*task)(void *arg);
	struct queue *next;
}queue;

typedef struct threadpool
{
	pthread_t pthread[N];
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	int busy_task;
	queue *head;
}threadpool;

threadpool *pool;

void *Worktask(void *arg)
{	
	while(1)
	{
		queue* ptask;

		pthread_mutex_lock(&pool->mutex);
		pthread_cond_wait(&pool->cond,&pool->mutex);
		if(pool->head!=NULL){
		ptask=pool->head;
		pool->head=ptask->next;
		ptask->task(ptask->arg);
		
		pool->busy_task--;
		}
		pthread_mutex_unlock(&pool->mutex);
	}

}

void *realwork(void *arg)
{
	printf("work start [%d]\n",*(int*)arg);
}

void queue_init(void* arg)
{
	queue *newtask;
	//-------------------------------------------
	pthread_mutex_lock(&pool->mutex);
	while(pool->busy_task>=N)
	{
			usleep(10000);
	}
	//------------------------------------------
	
	newtask=malloc(sizeof(queue));
	newtask->arg=arg;
	newtask->task=realwork;
	
	//insert tail queue	
	queue *ptask=pool->head;
	if(ptask==NULL)
	{
		pool->head=newtask;
	}
	else
	{
		while(ptask->next!=NULL)
		{
			ptask=ptask->next;
		}
			ptask->next=newtask;
	}
	pool->busy_task++;
	pthread_mutex_unlock(&pool->mutex);
	pthread_cond_signal(&pool->cond);

}

void pool_init()
{ 
	
	pool=malloc(sizeof(threadpool));
	pool->busy_task=0;
	pool->head=NULL;

	pthread_mutex_init(&pool->mutex,NULL);
	pthread_cond_init(&pool->cond,NULL);
	for(int i=0;i<N;i++){
	pthread_create(&pool->pthread[i],NULL,Worktask,NULL);
	}
}


void pool_destroy()
{
	queue *s;
	while(pool->head!=NULL)
	{
		s=pool->head;
		pool->head=s->next;
		free(s);
	}

	pthread_mutex_destroy(&pool->mutex);
	pthread_cond_destroy(&pool->cond);
	free(pool);
}


int main(int argv ,char **argc[])
{
	
	pool_init();//create 10 thread
	sleep(2);
	for(int i=0;i<20;i++)
	{
		queue_init((void*)&i);
	//	usleep(1);
	}
	sleep(1);

	pool_destroy();

	while(1)
	{
		sleep(1);
	}

} 
