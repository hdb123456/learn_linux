#include "sys/socket.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"

/*define area start*/
#define DEBUG 0
/*define area end*/

/*structure area start*/
/*structure area end*/
int main (int argc, char* argv[])
{
    if(argc != 3){
        printf("Usage: %s <ip> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    /*create socket*/
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        fprintf(stderr,"socket ERROR:%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    /*bind socket*/
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));//将主机字节序转为网络字节序大端,头文件为#include "arpa/inet.h"
#if DEBUG
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY是一个预定义的常量，其值通常为0，表示套接字将绑定到本机的所有IP地址上
#else
    int exchange = inet_aton(argv[1], &addr.sin_addr);
#endif
    if(exchange == 0)
    {
        printf("error inet_aton\n");
        exit(EXIT_FAILURE);
    }
   int ret_connect = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if( ret_connect == -1)
    {
        fprintf(stderr,"bind ERROR:%s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }

    char buf[BUFSIZ]={0};
    while(1)
    {
        printf("please input:\n");
        fgets(buf,BUFSIZ,stdin);
        int ret_write = write(sockfd,buf,strlen(buf));
        if(ret_write == -1)
        {
            fprintf(stderr,"write ERROR:%s\n",strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
 
    close(sockfd);
}
