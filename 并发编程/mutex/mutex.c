#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define N 2
FILE *stream;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;


int *func1(void * arg)
{
	pthread_detach(pthread_self());
	
	char c ;
	char a[]="first pthread\n";
	int i=0;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(i<strlen(a))
		{
			c=a[i];
			fputc(c,stream);
			usleep(1);
			i++;
		}
		pthread_mutex_unlock(&mutex);
		i=0;
		fflush(stream);
		usleep(1);
	}
}

int *func2(void * arg)
{
	pthread_detach(pthread_self());
	char c;
	int i=0;
	char a[]="second pthread\n";
	while(1)
	{	
		pthread_mutex_lock(&mutex);
		while(i<strlen(a))
		{
				c=a[i];
				fputc(c,stream);
				usleep(1);
				i++;
		}
		pthread_mutex_unlock(&mutex);
		i=0;
		usleep(1);
		fflush(stream);

	}

}

int main(int argc, char **argv)
{
	pthread_t head[N];
	stream=fopen("1.txt","a+");	
	if(stream==NULL)
	{
		perror("fopen error");
	}
	pthread_create(&head[0],NULL,(void*)func1,NULL);
	pthread_create(&head[1],NULL,(void*)func2,NULL);
		
	while(1)
	{
		sleep(1);
	}
}
