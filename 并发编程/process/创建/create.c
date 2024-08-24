#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// 正常终止进程，使用exit(EXIT_SUCCESS)
int main(char argc, int *argv[])
{
	int i=0;
	pid_t pid;

	pid= fork();//创建子进程

	if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

	if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) running...\n", getpid());
        sleep(2); // Simulate some work
        printf("Child process (PID: %d) exiting...\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        int status;
        printf("Parent process (PID: %d) waiting for child...\n", getpid());
        wait(&status); // Wait for child process to terminate
        printf("Parent process (PID: %d) reaped child process.\n", getpid());
    }

	return 0;
}

