进程的创建和回收
进程创建成功后返回子进程的PID，父进程返回0。init进程PID为1。
```cpp [pthread_create]
#include <signal.h>
       #include <stdint.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>

       int
       main(void)
       {
           pid_t pid;

           if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
               perror("signal");
               exit(EXIT_FAILURE);
           }//忽略子进程退出信号
           pid = fork();
           switch (pid) {
           case -1:
               perror("fork");
               exit(EXIT_FAILURE);
           case 0:
               puts("Child exiting.");
               exit(EXIT_SUCCESS);
           default:
               printf("Child is PID %jd\n", (intmax_t) pid);
               puts("Parent exiting.");
               exit(EXIT_SUCCESS);
           }
       }
```

ps



