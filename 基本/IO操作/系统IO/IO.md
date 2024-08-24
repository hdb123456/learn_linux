在 C 语言中，系统 I/O 主要涉及对操作系统提供的底层 I/O 功能的访问，这些功能比标准 I/O 函数更为底层且具有更高的性能。系统 I/O 通常涉及直接对文件描述符的操作，而不是通过 `FILE` 类型的流。

### 系统 I/O 函数

系统 I/O 函数提供了对文件、设备和其他 I/O 资源的直接控制，主要通过文件描述符进行操作。文件描述符是操作系统为每个打开的文件或设备分配的一个整数。

#### 1. 文件描述符

- **文件描述符**：在 Unix 和 Linux 系统中，文件描述符是一个非负整数，用于标识打开的文件或设备。标准输入、输出和错误通常分别对应于文件描述符 0、1 和 2。

#### 2. 打开和关闭文件

- **`open()`**
  - **用途**：打开一个文件或设备，返回一个文件描述符。
  - **原型**：`int open(const char *pathname, int flags, ... /* mode_t mode */);`
  - **参数**：
    - `pathname`：要打开的文件或设备的路径。
    - `flags`：打开文件的方式（如 `O_RDONLY` 只读模式、`O_WRONLY` 只写模式等）。
    - `mode`：文件权限（只有在创建文件时需要）。
  - **返回值**：成功时返回文件描述符，失败时返回 -1。
  - **示例**：

    ```c
    int fd = open("example.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
    }
    ```

- **`close()`**
  - **用途**：关闭一个文件描述符。
  - **原型**：`int close(int fd);`
  - **参数**：
    - `fd`：要关闭的文件描述符。
  - **返回值**：成功时返回 0，失败时返回 -1。
  - **示例**：

    ```c
    if (close(fd) == -1) {
        perror("Error closing file");
    }
    ```

#### 3. 读写文件

- **`read()`**
  - **用途**：从文件描述符中读取数据。
  - **原型**：`ssize_t read(int fd, void *buf, size_t count);`
  - **参数**：
    - `fd`：文件描述符。
    - `buf`：存储读取数据的缓冲区。
    - `count`：要读取的字节数。
  - **返回值**：成功时返回实际读取的字节数，失败时返回 -1。
  - **示例**：

    ```c
    char buffer[100];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("Error reading file");
    } else {
        printf("Read %ld bytes\n", bytesRead);
    }
    ```

- **`write()`**
  - **用途**：向文件描述符中写入数据。
  - **原型**：`ssize_t write(int fd, const void *buf, size_t count);`
  - **参数**：
    - `fd`：文件描述符。
    - `buf`：指向要写入数据的缓冲区。
    - `count`：要写入的字节数。
  - **返回值**：成功时返回实际写入的字节数，失败时返回 -1。
  - **示例**：

    ```c
    const char *text = "Hello, World!\n";
    ssize_t bytesWritten = write(fd, text, strlen(text));
    if (bytesWritten == -1) {
        perror("Error writing file");
    }
    ```

#### 4. 文件定位

- **`lseek()`**
  - **用途**：设置文件描述符的偏移量。
  - **原型**：`off_t lseek(int fd, off_t offset, int whence);`
  - **参数**：
    - `fd`：文件描述符。
    - `offset`：偏移量。
    - `whence`：起始位置（如 `SEEK_SET`、`SEEK_CUR`、`SEEK_END`）。
  - **返回值**：成功时返回新的偏移量，失败时返回 -1。
  - **示例**：

    ```c
    off_t pos = lseek(fd, 0, SEEK_SET);
    if (pos == -1) {
        perror("Error seeking file");
    }
    ```

#### 5. 文件状态

- **`fstat()`**
  - **用途**：获取文件描述符的状态信息。
  - **原型**：`int fstat(int fd, struct stat *statbuf);`
  - **参数**：
    - `fd`：文件描述符。
    - `statbuf`：指向 `struct stat` 的指针，用于存储文件状态。
  - **返回值**：成功时返回 0，失败时返回 -1。
  - **示例**：

    ```c
    struct stat fileInfo;
    if (fstat(fd, &fileInfo) == -1) {
        perror("Error getting file status");
    } else {
        printf("File size: %lld bytes\n", (long long)fileInfo.st_size);
    }
    ```

### 示例代码

下面是一个完整的示例代码，展示了如何使用系统 I/O 函数来打开、读取、写入和关闭文件。

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    int fd;
    char buffer[100];
    ssize_t bytesRead, bytesWritten;

    // 打开文件
    fd = open("example.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // 写入文件
    const char *text = "Hello, System I/O!\n";
    bytesWritten = write(fd, text, strlen(text));
    if (bytesWritten == -1) {
        perror("Error writing file");
        close(fd);
        return 1;
    }

    // 读取文件
    lseek(fd, 0, SEEK_SET); // 将文件指针移动到文件开头
    bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("Error reading file");
        close(fd);
        return 1;
    }
    buffer[bytesRead] = '\0'; // 添加字符串终止符
    printf("Read from file: %s", buffer);

    // 关闭文件
    if (close(fd) == -1) {
        perror("Error closing file");
        return 1;
    }

    return 0;
}
```

### 总结

系统 I/O 函数提供了对文件和设备的底层访问，相比于标准 I/O 函数（如 `fopen()`、`fread()` 和 `fwrite()`），它们提供了更直接的控制，允许更精细的文件操作和性能优化。这些函数对系统编程和性能敏感的应用尤其重要。