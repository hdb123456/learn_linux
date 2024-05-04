#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define N 4
FILE *stream;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_rwlock_t rwlock;
void* rwfunc(void* arg)
{
	pthread_detach(pthread_self());
	char data[30];
	while(1){
				rewind(stream);
		pthread_rwlock_rdlock(&rwlock);
		while(fgets(data,30,stream)!=NULL)
		{

			printf("%d thread,data is %s\n",(int)arg,data);
			usleep(1000);
		}
		pthread_rwlock_unlock(&rwlock);

	sleep(1);
	}




}
int *func1(void * arg)
{
	pthread_detach(pthread_self());
	
	char c ;
	char a[]="first pthread\n";
	int i=0;
	int len1;
	len1=strlen(a);
	printf("len1=%d\n",len1);
//	while(1)
	//{
	pthread_mutex_lock(&mutex);
		while(i<strlen(a))
		{
			c=a[i];
			fputc(c,stream);
			usleep(1);
			i++;
		}
	pthread_mutex_unlock(&mutex);
	//	i=0;
	fflush(stream);
		usleep(1);
	//}
}

int *func2(void * arg)
{
	pthread_detach(pthread_self());
	char c;
	int i=0;
	char a[]="second pthread\n";
	int len2;
	len2=strlen(a);
	printf("len2=%d\n",len2);
//	while(1)
	//{	
	pthread_mutex_lock(&mutex);
					while(i<strlen(a))
					{
						c=a[i];
						fputc(c,stream);
						usleep(1);
						i++;
					}
	//				i=0;
	pthread_mutex_unlock(&mutex);
					usleep(1);
					fflush(stream);

//	}

}

int main(int argc, char **argv)
{
	pthread_t head[N];
	stream=fopen("wdfunc.txt","a+");	
	if(stream==NULL)
	{
		perror("fopen error");
	}
	
	pthread_create(&head[1],NULL,(void*)rwfunc,1);
	pthread_create(&head[2],NULL,(void*)rwfunc,2);
	usleep(5);
	pthread_create(&head[3],NULL,(void*)func1,NULL);
	pthread_create(&head[4],NULL,(void*)func2,NULL);

	while(1)
	{
		sleep(1);
	}
}
