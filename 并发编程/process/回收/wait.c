//回收子进程，防止产生僵尸进程
//当父进程最终退出时，系统会清理僵尸进程。
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) running...\n", getpid());
        sleep(10); // Simulate long-running task
        printf("Child process (PID: %d) exiting...\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        printf("Parent process (PID: %d) continuing without waiting...\n", getpid());
        // Not calling wait(), so child becomes a zombie process
        sleep(20); // Simulate long-running task
        printf("Parent process (PID: %d) exiting...\n", getpid());

        //添加回收机制
        // Parent process should properly wait for child processes
        while (wait(NULL) > 0); // Reap all child processes
    }

    return 0;
}