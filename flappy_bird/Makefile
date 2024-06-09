# 编译器
CC=gcc
# 编译选项
CFLAGS=-g -Wall -Werror -pedantic
# 链接选项
LDFLAGS=-lncurses
# 目标文件
TARGET=myprogram

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): SIGALRM.o
	$(CC) -o $@ $^ $(LDFLAGS)

# 编译源文件
SIGALRM.o: SIGALRM.c
	$(CC) -c -o $@ $< $(CFLAGS)

# 清理生成的文件
clean:
	rm -f $(TARGET) *.o
