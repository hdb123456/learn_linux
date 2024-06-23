#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "unistd.h"


struct sockaddr_in{
    sa_family_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
};
struct in_addr{
    uint32_t s_addr;
};
int main (int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <ip> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    /*create socket*/
    int sockfd = socket(AF_INET,SOCK_STREAM,0)
    if(socket_fd == -1)
    {
        fprintf(stderr,"socket ERROR:%s\n",strrerror(error));
        exit(EXIT_FAILURE);
    }
    /*bind socket*/
    struct socketaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));//将主机字节序转为网络字节序大端,头文件为#include "arpa/inet.h"
    //addr.sin_addr.s_addr = argv[0];//INADDR_ANY是一个预定义的常量，其值通常为0，表示套接字将绑定到本机的所有IP地址上
    int exchange = inet_aton(argv[1], &addr.sin_addr);
    if(exchange == 0)
    {
        printf("error inet_aton\n");
        exit(EXIT_FAILURE);
    }
    ret_bind = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret_bind == -1)
    {
        fprintf(stderr,"bind ERROR:%s\n",strrerror(error));
        exit(EXIT_FAILURE);
    }
    /*listen socket*/
    listen_ret = listen(sockfd,5);
    if(listen_ret == -1)
    {
        fprintf(stderr,"listen ERROR:%s\n",strrerror(error));
        exit(EXIT_FAILURE);
    }
    /* accept client */
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int fd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
    if(fd==-1){
        fprintf(stderr,"accept ERROR:%s\n",strreror(error));
        exit(EXIT_FAILURE);
    }
    char buf[BUFSIZ]={0};
    ssize_t bytes_read = read(fd,buf,BUFSIZ);
    if(bytes_read==-1){
        fprintf(stderr,"read ERROR:%s\n",strrerror(error));
        exit(EXIT_FAILURE);
    }
    else{
        printf("recv:%s\n",buf);
    }

    close(fd);
}
/*test */
/*
```bash 1
    gcc TCP_connect.c -o TCP_connect
    ./TCP_connect
```
```bash 2
    nc 127.0.0.1 8080
    hello world
*/