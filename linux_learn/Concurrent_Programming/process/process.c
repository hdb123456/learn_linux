#include <stdio.h>
#include <unistd.h>

pid_t Fork(void);


int main(char argc, int *argv[])
{
	int i=0;
	pid_t pid;
	for(i=0;i<5;i++)
	{
		pid= fork();
		if(pid>0)
		{
			printf("father\r\n");
			sleep(5);
		}
		else if(pid=0)
		{
			printf("son\r\n");
			sleep(5);
		}
	}
exit(0);
}



pid_t Fork()
{
	pid_t pid;
	if((pid=fork())==-1)
	{
		unix_error("Fork error");
	}
	return pid;
}
