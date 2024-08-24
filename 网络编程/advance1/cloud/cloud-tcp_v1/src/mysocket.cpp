#include "mysocket.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define LISTEN_NUM 5
#define handle_error(msg)                                                              \
    do                                                                         \
    {                                                                          \
        std::cout << msg << std::endl;                                         \
        exit(1);                                                               \
    } while (0)



void arg_check(int argc, char *argv[])
{
    if (argc != 3)
    {
        handle_error("Usage: ./client <server_ip> <server_port>");
        exit(1);
    }
}

void my_socket::create_socket(void)
{
	int on=1;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        handle_error("Error creating socket");
        exit(1);
    }
    /*地址快速重用*/
	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))==-1)
	{
		perror("setsockopt");
	}
	std::cout<<"socket:"<<sfd<<std::endl;
}
int my_socket::bind_socket(char *argv[])
{
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[2]));
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    std::cout<<"bind_socket:"<<sfd<<std::endl;
    if (bind(sfd, (struct sockaddr *)&saddr, (socklen_t)sizeof(struct sockaddr_in)) < 0)
    {
        handle_error("Error binding socket");
        exit(1);
    }
    return sfd;
}
void my_socket::listen_socket(void)
{
    if(listen(sfd, LISTEN_NUM) == -1)
    {
        handle_error("Error listening");
    }
}
//从文件描述符中获取客户端信息
void get_peer(int fd)
{
	struct sockaddr_in addr;
	socklen_t size = sizeof(addr);
	getpeername(fd, (struct sockaddr*)&addr, &size);
	std::cout << "ip:"<< inet_ntoa(addr.sin_addr) << ", PORT: " << ntohs(addr.sin_port) << std::endl;
}

