#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main (int argc ,char **argv)
{
	char buff[32]={0};
	int fd;
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s <pipe_file>\n",argv[0]);
		return EXIT_FAILURE;
	}
	printf("ha%s\n",argv[1]);
	if((fd=open(argv[1],O_RDONLY))==-1)
	{
		fprintf(stderr,"open fifo failed\n");
		return EXIT_FAILURE;
	}
	while(1){
			read(fd,buff,32);
			printf("read buff:%s",buff);
			memset(buff,0,sizeof(buff));
	}
}
