CC = gcc
CFLAGS = -Wall -Wextra -Werror -fstack-protector -O2

target = udp_server udp_client

all: $(target)

# 隐式规则：为所有的.c文件生成.o目标文件
%.o: %.c
	$(CC) -c -o $@ $<
# 显式规则：编译链接目标程序
udp_server:udp_server.o
	$(CC) $(CFLAGS) -o $@ $<

udp_client:udp_client.o
	$(CC) $(CFLAGS) -o $@ $<

# 添加一个清理目标clean，用于安全清理编译产物
clean:
	rm -f $(target) *.o