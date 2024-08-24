`exec` 函数族是 Unix 和类 Unix 操作系统中用于执行新程序的一组系统调用。它们用于在当前进程中替换执行的程序，而不是创建一个新的进程。这个操作通常用于在已有进程的上下文中执行不同的程序。

### **`exec` 函数族概述**

`exec` 函数族包括一系列函数，这些函数具有相似的功能，但其参数传递方式略有不同。它们都用于将当前进程映像替换为新程序的映像，新的程序会完全取代当前的程序。

以下是常用的 `exec` 函数族成员：

1. **`execl`**：接受一个以空指针结尾的可变参数列表，表示新程序的命令行参数。
   ```c
   int execl(const char *path, const char *arg0, ..., (char *)NULL);
   ```
   - **path**：要执行的程序的路径。
   - **arg0**：第一个参数（通常是程序名）。
   - **...**：其他参数，以空指针 `NULL` 结尾。

2. **`execle`**：与 `execl` 类似，但额外接受一个环境变量数组作为参数。
   ```c
   int execle(const char *path, const char *arg0, ..., (char *)NULL, char *const envp[]);
   ```
   - **envp**：环境变量数组，以 `NULL` 结尾。

3. **`execlp`**：与 `execl` 类似，但允许在系统的 `PATH` 环境变量中查找程序。
   ```c
   int execlp(const char *file, const char *arg0, ..., (char *)NULL);
   ```
   - **file**：程序的文件名，可以在 `PATH` 中查找。

4. **`execvp`**：与 `execlp` 类似，但接受一个参数数组。
   ```c
   int execvp(const char *file, char *const argv[]);
   ```
   - **argv**：参数数组，以 `NULL` 结尾。

5. **`execv`**：接受一个参数数组，类似于 `execl` 和 `execle`。
   ```c
   int execv(const char *path, char *const argv[]);
   ```
   - **argv**：参数数组，以 `NULL` 结尾。

6. **`execve`**：最底层的 `exec` 函数，直接调用操作系统的 `exec` 系统调用。
   ```c
   int execve(const char *pathname, char *const argv[], char *const envp[]);
   ```
   - **pathname**：要执行的程序路径。
   - **argv**：参数数组，以 `NULL` 结尾。
   - **envp**：环境变量数组，以 `NULL` 结尾。

### **使用示例**

以下是一些示例代码，展示如何使用这些 `exec` 函数。

**使用 `execl` 执行 `/bin/ls`**

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Before exec\n");
    execl("/bin/ls", "ls", "-l", (char *)NULL);
    // execl 成功时不会返回
    perror("execl failed");
    return 1;
}
```

**使用 `execvp` 执行 `ls`**

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    char *args[] = {"ls", "-l", NULL};
    printf("Before exec\n");
    execvp("ls", args);
    // execvp 成功时不会返回
    perror("execvp failed");
    return 1;
}
```

**使用 `execve` 执行 `/bin/ls` 并设置环境变量**

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    char *envp[] = {"PATH=/bin", NULL};
    char *args[] = {"ls", "-l", NULL};
    printf("Before exec\n");
    execve("/bin/ls", args, envp);
    // execve 成功时不会返回
    perror("execve failed");
    return 1;
}
```

### **`exec` 系列函数的行为**

1. **替换当前进程映像**：执行 `exec` 系列函数会用新程序替换当前进程的映像，包括代码、数据和堆栈。原进程的 PID 和文件描述符等信息会保留，但程序代码和数据会被新程序的代码和数据替代。

2. **不会返回**：如果 `exec` 调用成功，新的程序会接管进程，原进程的代码不会继续执行，因此 `exec` 系列函数在成功时不会返回。失败时会返回 -1，并设置 `errno` 以指示错误原因。

3. **资源继承**：执行 `exec` 函数时，打开的文件描述符和信号处理器会被继承。新程序可以继续使用这些资源，除非显式关闭或修改。

### **总结**

`exec` 系列函数提供了一种强大的机制来替换当前进程的程序。这些函数允许在当前进程的上下文中执行不同的程序，而不是创建一个新的进程。它们的不同变种适用于不同的场景和需求，比如传递环境变量、在 `PATH` 中查找程序等。