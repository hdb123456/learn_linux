#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>


int sock_fd = -1;
void Call_handle(void);
void handle_SIGINT(int sig);
int main (int argc, char *argv[])
{
    if(argc!=3)
    {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    Call_handle();
    sock_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd<0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family=AF_INET;
    client_addr.sin_port=htons(atoi(argv[2]));//htons(PORT);
    //client_addr.sin_addr.s_addr=inet_addr(SERVER_IP);
    if ( inet_aton(argv[1], &client_addr.sin_addr)==0)
		{
			perror("inet_aton");
			return -1;
		}
    if(connect(sock_fd, (struct sockaddr *)&client_addr, sizeof(client_addr))<0)
    {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Connect to server successfully!\n");
    char recvBuf[BUFSIZ]={0};
    recv(sock_fd, recvBuf, sizeof(recvBuf), 0);
    printf("receive server's msg:%s\n", recvBuf);

    char buf[BUFSIZ]={0};
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        printf("Please enter your message: ");
        fgets(buf, sizeof(buf), stdin);
        if(send(sock_fd, buf, sizeof(buf), 0)<0)
        {
            perror("send");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

    }
           
   
}


void Call_handle(void)
{
	struct sigaction act;
	act.sa_handler = handle_SIGINT;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
}

void handle_SIGINT(int sig){
	
	if(sock_fd>0){
		close(sock_fd);
	}
	printf("SIGINT\n");
	exit(EXIT_SUCCESS);
}