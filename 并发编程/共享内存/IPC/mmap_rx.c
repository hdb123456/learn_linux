#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argv, char **argc)
{
	char* addr;
	int fd;

	if (argv==2)
	{	
		printf("argc[1]:%s\n",argc[1]);
		if((fd= open(argc[1],O_RDWR))<0)
			perror("fd failed");
	}
	else
	{
		perror("argc is invalid");
	}
	int len=lseek(fd,0,SEEK_END);
        if((addr=(char*)mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==(void*)-1)
	{
		perror("mmap failed");
	}
	sleep(1);
	close(fd);

	while(1)
	{
		printf("%s\n",addr);
		sleep(2);
	}
}
