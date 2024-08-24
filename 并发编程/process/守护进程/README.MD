守护进程（Daemon Process）是指在后台运行的、无交互的进程，通常用于提供系统服务或执行后台任务。它们在系统启动时启动，通常在整个系统运行期间持续存在。

实现守护进程通常包括以下步骤：

### **1. 创建新的进程**

守护进程通常由一个父进程创建。这个父进程在创建守护进程后，通常会退出，这样守护进程就会成为孤儿进程，并由 `init` 进程（或系统的等效进程）接管。

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

pid_t fork(void);

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process exits, child continues as daemon
        exit(EXIT_SUCCESS);
    }

    // Child process continues
    printf("Daemon process started\n");
    return 0;
}
```

### **2. 创建新的会话**

创建新的会话使守护进程脱离终端控制，确保它不会受到用户终端的影响。

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Parent exits
    }

    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // Daemon continues
    printf("Daemon process started\n");
    return 0;
}
```

### **3. 处理当前工作目录**

守护进程通常将当前工作目录更改为根目录，以避免锁定卸载的文件系统。

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Parent exits
    }

    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // Change working directory to root
    if (chdir("/") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // Daemon continues
    printf("Daemon process started\n");
    return 0;
}
```

### **4. 关闭标准文件描述符**

守护进程通常会关闭标准输入、标准输出和标准错误，以避免不必要的终端交互。

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Parent exits
    }

    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Daemon continues
    printf("Daemon process started\n");
    return 0;
}
```

### **5. 处理信号**

守护进程应处理各种信号，如 `SIGTERM`，以便能够优雅地关闭或执行清理操作。

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signal) {
    // Handle signal
    printf("Daemon received signal %d\n", signal);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Parent exits
    }

    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Set up signal handling
    signal(SIGTERM, signal_handler);

    // Daemon main loop
    while (1) {
        sleep(60); // Simulate doing work
    }

    return 0;
}
```

### **5. 设置文件权限掩码**
守护进程通常需要创建新文件，设置文件权限掩码可以确保新创建的文件具有正确的权限。

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signal) {
    // Handle signal
    printf("Daemon received signal %d\n", signal);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Parent exits
    }

    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Set up signal handling
    signal(SIGTERM, signal_handler);

    // Set file mode creation mask
    umask(0); // Set umask to 0 to ensure files have full permissions

    // Daemon main loop
    while (1) {
        // Simulate doing work
        sleep(60); // Sleep for 60 seconds
    }

    return 0;
}
```

#### 为什么设置 umask 为 0

将 umask 设置为 0 确保新创建的文件和目录没有权限限制，使其具有最高权限。这对于守护进程是有用的，因为它允许进程完全控制新创建的文件的权限，尤其是在它需要创建临时文件或日志文件时。你也可以根据需要设置不同的 umask 值来控制权限，例如：

```c
复制代码
umask(S_IWGRP | S_IWOTH); // 设置 umask 以限制某些权限
```
这将限制新文件的组写权限和其他用户写权限。


### **6. 实现主循环**

守护进程通常在一个主循环中运行，执行它的主要功能，比如监听网络请求、处理文件等。

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_handler(int signal) {
    // Handle signal
    printf("Daemon received signal %d\n", signal);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Parent exits
    }

    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    if (chdir("/") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // Close standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Set up signal handling
    signal(SIGTERM, signal_handler);

    // Set file mode creation mask
    umask(0); // Set umask to 0 to ensure files have full permissions

    // Daemon main loop
    while (1) {
        // Simulate doing work
        sleep(60); // Sleep for 60 seconds
    }

    return 0;
}
```

### **总结**

实现一个守护进程的关键步骤包括：
1. **创建新的进程**：使守护进程脱离终端。
2. **创建新的会话**：使守护进程脱离终端控制。
3. **更改工作目录**：避免锁定卸载的文件系统。
4. **关闭标准文件描述符**：避免与终端交互。
5. **处理信号**：优雅地处理系统信号。
6. **设置文件权限掩码**：确保新创建的文件具有正确的权限。
7. **实现主循环**：执行守护进程的主要任务。

这些步骤确保守护进程能够在后台稳定地运行并提供所需的服务。