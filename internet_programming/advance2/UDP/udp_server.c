#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define len 1024
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE);}while(0)

int main (int argc, char *argv[]) {
    if(argc != 3)
    {
        printf("usage: %s <ip> <port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    char buff[len]={0};
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sfd == -1)
    {
        handle_error("socket");
    }   
    
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    if(inet_aton(argv[1], &servaddr.sin_addr)==0)
    {
        close(sfd);
        handle_error("inet_aton");
    }
    if(bind(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)
    {
        close(sfd);
        handle_error("bind");
    }
    while(1)
    {
        memset(buff,0,len);
        if(recvfrom(sfd,buff,len,0,NULL,NULL)==-1)
        {
            close(sfd);
            handle_error("recvfrom");
        }
        printf("recvfrom:%s",buff);
    }
    close(sfd);
}
