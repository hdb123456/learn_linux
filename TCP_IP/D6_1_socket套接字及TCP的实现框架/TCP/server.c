#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"

int main (int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <ip> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    /*create socket*/
    int sockfd=create_socket;
    /*bind socket*/
   bind_address(&sockfd,argv);
    /*listen socket*/
    int listen_ret = listen(sockfd,5);
    if(listen_ret == -1)
    {
        fprintf(stderr,"listen ERROR:%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* accept client */
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int fd = accept(sockfd, (struct sockaddr *)&client_addr, &len);
    if(fd==-1){
        fprintf(stderr,"accept ERROR:%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    char buf[BUFSIZ]={0};
    while(1)
    {
        memset(buf,0,BUFSIZ);
        ssize_t bytes_read = read(fd,buf,BUFSIZ);
        if(bytes_read==-1){
            fprintf(stderr,"read ERROR:%s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }
        else{
            printf("recv:%s",buf);
        }
    }
    close(fd);
    close(sockfd);
}

int create_socket()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        fprintf(stderr,"socket ERROR:%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    return sockfd;
}
void bind_address(int sockfd,char* argv[])
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));//将主机字节序转为网络字节序大端,头文件为#include "arpa/inet.h"
    //addr.sin_addr.s_addr = argv[0];//addr.sin_addr.s_addr=INADDR_ANY,INADDR_ANY是一个预定义的常量，其值通常为0，表示套接字将绑定到本机的所有IP地址上
    int exchange = inet_aton(argv[1], &addr.sin_addr);
    if(exchange == 0)
    {
        printf("error inet_aton\n");
        exit(EXIT_FAILURE);
    }
    int ret_bind = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret_bind == -1)
    {
        fprintf(stderr,"bind ERROR:%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
}