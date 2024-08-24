在 C 语言中，文件 I/O（输入/输出）通过 `<stdio.h>` 提供的一组函数来实现。这些函数使得程序可以读取和写入文件。下面是 C 语言中文件 I/O 操作的详细介绍，包括文件的打开、关闭、读取、写入和错误处理等操作。

### 文件操作函数

#### 1. 打开和关闭文件

- **`fopen()`**
  - **用途**：打开一个文件，并返回一个文件流。
  - **原型**：`FILE *fopen(const char *filename, const char *mode);`
  - **参数**：
    - `filename`：要打开的文件名。
    - `mode`：文件的打开模式（如 `"r"` 读取模式、`"w"` 写入模式等）。
  - **示例**：

    ```c
    FILE *file = fopen("example.txt", "r"); // 以只读模式打开文件
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    ```

- **`fclose()`**
  - **用途**：关闭一个文件流。
  - **原型**：`int fclose(FILE *stream);`
  - **参数**：
    - `stream`：要关闭的文件流。
  - **示例**：

    ```c
    if (fclose(file) != 0) {
        perror("Error closing file");
    }
    ```

#### 2. 读取文件

- **`fread()`**
  - **用途**：从文件流中读取数据。
  - **原型**：`size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);`
  - **参数**：
    - `ptr`：指向存储读取数据的内存块的指针。
    - `size`：每个数据项的大小（以字节为单位）。
    - `nmemb`：要读取的数据项的数量。
    - `stream`：文件流。
  - **返回值**：实际读取的数据项数。
  - **示例**：

    ```c
    char buffer[100];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    if (bytesRead < sizeof(buffer)) {
        if (feof(file)) {
            printf("End of file reached.\n");
        } else if (ferror(file)) {
            perror("Error reading file");
        }
    }
    ```

- **`fgets()`**
  - **用途**：从文件流中读取一行文本。
  - **原型**：`char *fgets(char *str, int n, FILE *stream);`
  - **参数**：
    - `str`：存储读取数据的缓冲区。
    - `n`：缓冲区的大小。
    - `stream`：文件流。
  - **返回值**：指向 `str` 的指针，读取失败或到达文件末尾时返回 `NULL`。
  - **示例**：

    ```c
    char line[256];
    if (fgets(line, sizeof(line), file) != NULL) {
        printf("Read line: %s", line);
    } else {
        if (feof(file)) {
            printf("End of file reached.\n");
        } else if (ferror(file)) {
            perror("Error reading file");
        }
    }
    ```

#### 3. 写入文件

- **`fwrite()`**
  - **用途**：向文件流中写入数据。
  - **原型**：`size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);`
  - **参数**：
    - `ptr`：指向要写入的数据的内存块。
    - `size`：每个数据项的大小（以字节为单位）。
    - `nmemb`：要写入的数据项的数量。
    - `stream`：文件流。
  - **返回值**：实际写入的数据项数。
  - **示例**：

    ```c
    const char *text = "Hello, World!\n";
    size_t bytesWritten = fwrite(text, 1, strlen(text), file);
    if (bytesWritten < strlen(text)) {
        perror("Error writing file");
    }
    ```

- **`fprintf()`**
  - **用途**：向文件流中写入格式化的数据。
  - **原型**：`int fprintf(FILE *stream, const char *format, ...);`
  - **参数**：
    - `stream`：文件流。
    - `format`：格式化字符串。
    - `...`：要格式化的数据。
  - **示例**：

    ```c
    int number = 42;
    fprintf(file, "The answer is %d\n", number);
    ```

#### 4. 文件定位和状态

- **`fseek()`**
  - **用途**：设置文件流的当前位置。
  - **原型**：`int fseek(FILE *stream, long offset, int whence);`
  - **参数**：
    - `stream`：文件流。
    - `offset`：相对于 `whence` 的偏移量。
    - `whence`：起始位置（如 `SEEK_SET`、`SEEK_CUR`、`SEEK_END`）。
  - **示例**：

    ```c
    fseek(file, 0, SEEK_SET); // 将文件指针移动到文件开头
    ```

- **`ftell()`**
  - **用途**：获取文件流的当前位置。
  - **原型**：`long ftell(FILE *stream);`
  - **参数**：
    - `stream`：文件流。
  - **返回值**：当前位置的偏移量。
  - **示例**：

    ```c
    long pos = ftell(file);
    printf("Current file position: %ld\n", pos);
    ```

- **`rewind()`**
  - **用途**：将文件流的当前位置重新设置为文件开头。
  - **原型**：`void rewind(FILE *stream);`
  - **参数**：
    - `stream`：文件流。
  - **示例**：

    ```c
    rewind(file); // 将文件指针移到文件开头
    ```

#### 5. 文件状态和错误处理

- **`feof()`**
  - **用途**：检查文件流是否到达文件末尾。
  - **原型**：`int feof(FILE *stream);`
  - **参数**：
    - `stream`：文件流。
  - **返回值**：如果到达文件末尾，返回非零值；否则返回零。
  - **示例**：

    ```c
    if (feof(file)) {
        printf("End of file reached.\n");
    }
    ```

- **`ferror()`**
  - **用途**：检查文件流是否发生错误。
  - **原型**：`int ferror(FILE *stream);`
  - **参数**：
    - `stream`：文件流。
  - **返回值**：如果发生错误，返回非零值；否则返回零。
  - **示例**：

    ```c
    if (ferror(file)) {
        perror("File error");
    }
    ```

- **`clearerr()`**
  - **用途**：清除文件流的错误标志。
  - **原型**：`void clearerr(FILE *stream);`
  - **参数**：
    - `stream`：文件流。
  - **示例**：

    ```c
    clearerr(file); // 清除错误标志
    ```

### 总结

C 语言的文件 I/O 函数为程序提供了强大的文件操作能力，包括文件的打开、关闭、读取、写入、定位以及错误处理。理解和正确使用这些函数对于进行文件操作和处理数据是非常重要的。