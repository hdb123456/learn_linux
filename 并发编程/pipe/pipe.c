#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void write_from_pipe(int file)
{
	FILE *stream;
	stream=fdopen(file,"w");
	fprintf(stream,"hello world\n");
	fclose(stream);
}
void read_from_pipe(int file)
{
	FILE *stream;
	int c;
	stream=fdopen(file,"r");
	while((c=fgetc(stream))!=EOF)
					putchar(c);
	fclose(stream);
}


int main (int argv ,char **argc)
{
	pid_t pid;
	int mypipe[2];

	if(pipe(mypipe))
	{
		fprintf(stderr,"Pipe failed\n");
		return EXIT_FAILURE;
	}

	pid=fork();
	if(pid <(pid_t)0)
	{
		fprintf(stderr,"Fork failed.\n");
		return EXIT_FAILURE;
	}
	else if(pid>(pid_t)0)
	{
		close(mypipe[0]);
		write_from_pipe(mypipe[1]);
		return EXIT_SUCCESS;
	}
	else 
	{
		close(mypipe[1]);
		read_from_pipe(mypipe[0]);
		return EXIT_SUCCESS;
	}

}
