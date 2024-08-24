#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has=PTHREAD_COND_INITIALIZER;

struct taxi
{
	int num;
	struct taxi *next;
};

struct taxi *head=NULL;

void *product(void *arg)
{
	pthread_detach(pthread_self());
	int i=1;
	struct taxi *pro;
	while(1)
	{

		pthread_mutex_lock(&mutex);
		pro=malloc(sizeof(struct taxi));
		pro->num=i++;	
		pro->next=head;
		head=pro;
		printf("pthread %d product-[%d]\n",*(int*)arg,pro->num);
		pthread_cond_signal(&has);
		pthread_mutex_unlock(&mutex);

		sleep(1);
	}


}
void*take(void *arg)
{
	pthread_detach(pthread_self());
		struct taxi *ts;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		
		while(head==NULL){
			pthread_cond_wait(&has,&mutex);
		}
		ts=head;
		head=head->next;
		
		printf("pthread %d take-[%d]\n",*(int*)arg,ts->num);
		free(ts);

		pthread_mutex_unlock(&mutex);
	}


}

int main(int argv ,char *argc[])
{
	int value[2]={1,2};
	pthread_t head[2];
	pthread_create(&head[0],NULL,product,(void*)(&value[0]));
	usleep(2);
	pthread_create(&head[1],NULL,take,(void*)(&value[1]));

while(1)
{
sleep(1);
}
}
