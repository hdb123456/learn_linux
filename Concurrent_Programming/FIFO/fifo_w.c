#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define N 32

int main (int argc ,char **argv)
{
	char buff[N];
	int fd;
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s <pipe_file>\n",argv[0]);
		return EXIT_FAILURE;
	}
	printf("ha%s\n",argv[1]);
	if((fd=open(argv[1],O_WRONLY))==-1)
	{
		fprintf(stderr,"open fifo failed\n");
		return EXIT_FAILURE;
	}
while(1){
		fgets(buff,N,stdin);
		write(fd,buff,strlen(buff));
	}
}
