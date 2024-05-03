#include "stdio.h"
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char**argv)
{			
	int fd;
	void*addr;
	if(argc!=2)
	{
		perror("arguments");
		return EXIT_FAILURE;
	}
	printf("argv[1]=%s",argv[1]);
	fd=open(argv[1],O_RDWR);
	if(fd==-1)
	{
		perror("open");
		return 0;
	}
//int len=lseek(fd,0,SEEK_END);
	addr=mmap(NULL,6000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	 if(addr==MAP_FAILED)
	 {
			perror("mmap");
			return 0;
	 }

	 memcpy(addr+5000,"234235453454535345sdfsfsdfgregs",50);
	 printf("addr :%s\n",(char*)addr);
}
