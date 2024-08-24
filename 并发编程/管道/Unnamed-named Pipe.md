有名管道（Named Pipe）和无名管道（Unnamed Pipe）是 Unix/Linux 系统中用于进程间通信（IPC）的两种管道类型。它们各自有不同的特点和使用场景。下面是对这两种管道的详细说明。

### 1. 无名管道（Unnamed Pipe）

#### 特点
- **短期使用**：无名管道通常在创建它们的进程的生命周期内有效，一旦进程结束，管道也会被销毁。
- **进程间通信**：无名管道只能在具有亲缘关系的进程之间进行通信（例如父子进程）。它们不能在不相关的进程之间通信。
- **创建和使用**：通过 `pipe()` 系统调用创建，返回两个文件描述符，一个用于读，一个用于写。

#### 创建和使用示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2]; // 文件描述符数组
    pid_t pid;
    
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子进程
        close(fd[1]); // 关闭写端
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));
        printf("Child read: %s\n", buffer);
        close(fd[0]);
    } else { // 父进程
        close(fd[0]); // 关闭读端
        char* msg = "Hello from parent";
        write(fd[1], msg, sizeof(msg));
        close(fd[1]);
    }

    return 0;
}
```

### 2. 有名管道（Named Pipe）

#### 特点
- **长期存在**：有名管道存在于文件系统中，因此它们的生命周期独立于进程。可以在不相关的进程之间进行通信。
- **路径**：有名管道通过在文件系统中创建一个特殊的文件来实现，通常用 `mkfifo()` 函数创建。
- **创建和使用**：有名管道的创建和使用过程类似于普通文件，可以通过路径名进行访问。

#### 创建和使用示例

1. **创建有名管道**

```bash
mkfifo /tmp/myfifo
```

2. **写入有名管道的示例**

```bash
echo "Hello from writer" > /tmp/myfifo
```

3. **读取有名管道的示例**

```bash
cat /tmp/myfifo
```

4. **C 语言中创建和使用有名管道**

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    const char* fifo_path = "/tmp/myfifo";
    
    // 创建有名管道
    mkfifo(fifo_path, 0666);

    // 写入有名管道
    fd = open(fifo_path, O_WRONLY);
    write(fd, "Hello from writer", 18);
    close(fd);

    // 读取有名管道
    fd = open(fifo_path, O_RDONLY);
    char buffer[100];
    read(fd, buffer, sizeof(buffer));
    printf("Reader read: %s\n", buffer);
    close(fd);

    return 0;
}
```

### 区别总结

1. **命名**：
   - **无名管道**：没有名称，存在于进程内存中。
   - **有名管道**：具有文件系统中的路径名。

2. **通信范围**：
   - **无名管道**：仅限于有亲缘关系的进程（如父子进程）。
   - **有名管道**：可以在任意进程之间进行通信。

3. **生命周期**：
   - **无名管道**：随创建它的进程的生命周期存在。
   - **有名管道**：独立于进程生命周期，存在于文件系统中。

4. **创建和管理**：
   - **无名管道**：通过 `pipe()` 系统调用创建。
   - **有名管道**：通过 `mkfifo()` 系统调用创建，通常与文件系统中的路径相关联。

这两种管道各有优缺点，选择使用哪种取决于具体的应用场景和需求。