#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int PORT=8080;
#define SERVER_IP "127.0.0.1"
//#define BUF_SIZE 1024
void check(const char *message, int sock_fd);
int main (int argc, char *argv[])
{
    int sock_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd<0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        struct sockaddr_in server_addr;
        server_addr.sin_family=AF_INET;
        server_addr.sin_port=htons(PORT);
        server_addr.sin_addr.s_addr=inet_addr(SERVER_IP);
        if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0)
       {
           check("connect", sock_fd);
       }
        char buf[BUFSIZ]={0};
        while(1)
        {
            memset(buf, 0, sizeof(buf));
            printf("Please enter your message: ");
            fgets(buf, sizeof(buf), stdin);
            if(send(sock_fd, buf, sizeof(buf), 0)<0)
            {
                check("send", sock_fd);
            }
        }
           
   }
}


void check(const char *message, int sock_fd)
{
	perror(message);
	if(sock_fd>=0)
		close(sock_fd);

	exit(EXIT_FAILURE);
}