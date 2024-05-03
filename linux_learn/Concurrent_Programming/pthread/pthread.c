#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

int *func(void * arg)
{
	printf("phtread id is %lu\r\n",pthread_self());
//	return NULL;
printf("arg is %d\r\n",(int)arg);
pthread_exit(NULL);
}



int main(int argc, char **argv)
{
	pthread_t head[N];
int i=0;
	for(i=0;i<5;i++)
	{
		
	pthread_create(&head[i],NULL,(void*)func,(void*)i);
	//sleep(1);
	printf("this is %d pthread\r\n",i);

	}
	while(1)
	{
		sleep(1);
	}
}
