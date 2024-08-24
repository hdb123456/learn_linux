#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define len 1024
void Send(char* argv[]);
int main(int argc, char* argv[])
{
	if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	Send(argv);
	return 0;
}

void Send(char* argv[])
{
    char buff[len] = {0};	
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);

	int sfd = socket(AF_INET, SOCK_DGRAM,0);
	if(sfd == -1)
	{
		handle_error("socket");
	}
	
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(atoi(argv[2]));
	if(inet_aton(argv[1], &cliaddr.sin_addr) == 0)
	{
		close(sfd);
		handle_error("inet_aton");
	}  
	while(1)
	{
		memset(buff, 0, len);
		fgets(buff,len,stdin);
		sendto(sfd, buff,strlen(buff),0,(struct sockaddr *)&cliaddr,addrlen);
	}
	close(sfd);
}
