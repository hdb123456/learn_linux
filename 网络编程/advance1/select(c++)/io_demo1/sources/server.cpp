#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define handle_error(msg) \
    do { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    } while (0)

char rbuf[50], sbuf[100];
int main (int argc, char *argv[]) 
{
    int server_fd = -1, ret;
    struct sockaddr_in server_addr, client_addr;
    char on = 1;
    //设置地址、ip信息


    socklen_t size = sizeof(struct sockaddr_in);
    memset(&server_addr, 0, size);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) 
        handle_error("socket");
    //设置端口复用
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&on, (socklen_t)sizeof(on));
    //绑定套接字
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        close(server_fd);
        handle_error("bind");
    }

    int val = sizeof(struct sockaddr);
    while (1) {
        puts("waiting for client...");
        ret = recvfrom (server_fd, rbuf, 50, 0, (struct sockaddr*)&client_addr, (socklen_t*)&val);
        if(ret<0)
            perror("recvfrom");
        printf("receive data:%s\n",rbuf);
        sprintf(sbuf, "server has receive data(%s)\n", rbuf);
        ret = sendto(server_fd, sbuf, strlen(sbuf), 0, (struct sockaddr*)&client_addr, sizeof(struct sockaddr));
        memset(rbuf, 0, 50);
        memset(sbuf, 0, 100);
    }
    close(server_fd);
    getchar();
    return 0;
}