#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argv, char **argc)
{
	char* addr;
	int fd;
	int i=0;
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
        if((addr=(char*)mmap(NULL,20,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==(void*)-1)
	{
		perror("mmap failed");
	}
	sleep(1);
	close(fd);

	while(1)
	{	
		 char str[30];
		printf("input string:");
		fgets(str,sizeof(str),stdin);

		memcpy(addr,str,30);
		addr+=strlen(str);
		sleep(2);
	}
}
