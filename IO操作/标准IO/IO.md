在 C 语言中，标准 I/O（输入/输出）是通过标准库 `<stdio.h>` 提供的一组函数来实现的。这些函数用于处理程序的输入、输出以及错误信息。以下是对 C 语言标准 I/O 函数的详细介绍。

### 1. 标准输入（`stdin`）

标准输入流是用于从外部设备（通常是键盘）读取数据的。

#### 输入函数

- **`scanf()`**
  - **用途**：从标准输入流读取格式化的数据。
  - **原型**：`int scanf(const char *format, ...);`
  - **示例**：

    ```c
    int x;
    scanf("%d", &x); // 读取一个整数
    ```

- **`fgets()`**
  - **用途**：从指定的文件流（如 `stdin`）读取一行文本。
  - **原型**：`char *fgets(char *str, int n, FILE *stream);`
  - **示例**：

    ```c
    char buffer[100];
    fgets(buffer, sizeof(buffer), stdin); // 读取一行文本
    ```

- **`getchar()`**
  - **用途**：从标准输入流读取一个字符。
  - **原型**：`int getchar(void);`
  - **示例**：

    ```c
    int c;
    c = getchar(); // 读取一个字符
    ```

- **`getc()`**
  - **用途**：从指定的文件流（如 `stdin`）读取一个字符。
  - **原型**：`int getc(FILE *stream);`
  - **示例**：

    ```c
    int c;
    c = getc(stdin); // 读取一个字符
    ```

### 2. 标准输出（`stdout`）

标准输出流是用于将数据输出到外部设备（通常是屏幕）上。

#### 输出函数

- **`printf()`**
  - **用途**：向标准输出流写入格式化的数据。
  - **原型**：`int printf(const char *format, ...);`
  - **示例**：

    ```c
    printf("Hello, %s!\n", "world"); // 打印格式化字符串
    ```

- **`puts()`**
  - **用途**：向标准输出流写入一个字符串，并在末尾添加换行符。
  - **原型**：`int puts(const char *str);`
  - **示例**：

    ```c
    puts("Hello, world!"); // 打印字符串并换行
    ```

- **`putchar()`**
  - **用途**：向标准输出流写入一个字符。
  - **原型**：`int putchar(int c);`
  - **示例**：

    ```c
    putchar('A'); // 打印字符 'A'
    ```

- **`fputc()`**
  - **用途**：向指定的文件流（如 `stdout`）写入一个字符。
  - **原型**：`int fputc(int c, FILE *stream);`
  - **示例**：

    ```c
    fputc('A', stdout); // 打印字符 'A' 到标准输出
    ```

### 3. 标准错误（`stderr`）

标准错误流是用于输出错误信息，通常与标准输出设备相同，但可以被重定向到不同的位置。

#### 错误输出函数

- **`fprintf()`**
  - **用途**：将格式化的数据输出到指定的文件流（如 `stderr`）。
  - **原型**：`int fprintf(FILE *stream, const char *format, ...);`
  - **示例**：

    ```c
    fprintf(stderr, "Error: %s\n", error_message); // 打印错误信息
    ```

- **`perror()`**
  - **用途**：根据 `errno` 打印错误信息。
  - **原型**：`void perror(const char *s);`
  - **示例**：

    ```c
    perror("Error occurred"); // 打印与 errno 相关的错误信息
    ```

### 4. 文件操作

标准 I/O 函数还包括用于文件操作的函数，这些函数通过文件流进行输入/输出操作。

- **`fopen()`**
  - **用途**：打开一个文件并返回文件流。
  - **原型**：`FILE *fopen(const char *filename, const char *mode);`
  - **示例**：

    ```c
    FILE *file = fopen("example.txt", "w"); // 打开文件用于写入
    ```

- **`fclose()`**
  - **用途**：关闭一个文件流。
  - **原型**：`int fclose(FILE *stream);`
  - **示例**：

    ```c
    fclose(file); // 关闭文件流
    ```

- **`fread()`**
  - **用途**：从文件流中读取数据。
  - **原型**：`size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);`
  - **示例**：

    ```c
    char buffer[100];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), file); // 从文件中读取数据
    ```

- **`fwrite()`**
  - **用途**：向文件流中写入数据。
  - **原型**：`size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);`
  - **示例**：

    ```c
    fwrite(buffer, 1, bytes_to_write, file); // 向文件中写入数据
    ```

- **`fseek()`** 和 **`ftell()`**
  - **用途**：用于文件指针的位置控制。
  - **原型**：
    - `int fseek(FILE *stream, long offset, int whence);`
    - `long ftell(FILE *stream);`
  - **示例**：

    ```c
    fseek(file, 0, SEEK_SET); // 将文件指针移动到文件开头
    long position = ftell(file); // 获取文件指针位置
    ```

### 总结

标准 I/O 函数提供了一组强大的工具，用于处理程序的输入和输出。这些函数使得与用户和文件进行交互变得简单而直观。掌握这些函数的用法是编写高效和健壮的 C 语言程序的基础。