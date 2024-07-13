#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>

uint16_t sockConnect=-1;
uint8_t server_fd = -1;

void Call_handle(void);
void handle_SIGINT(int sig);
int init(int server_fd);
bool conmmunication(int server_fd);

//#ifdef N
#define N 1
//#endif

int main (int argc,char *argv[])
{	
	Call_handle();
	bool flag = init(server_fd);
	if(flag==0)
	{
		perror("init");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
int init(int server_fd)
{	
	struct sockaddr_in server_addr;
	uint8_t on = 1;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd<0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	/*地址快速重用*/
	// if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))==-1)
	// {
	// 	perror("setsockopt");
	// 	return 0;
	// }
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	/* 设置服务器地址结构体的成员 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);//htons(atoi(argv[2]))
	#if N==1 
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(argv[1])
	#else
		if ( inet_aton(argv[1], &server_addr.sin_addr)==0)
		{
			perror("inet_aton");
			return 0;
		}
	#endif
	/*将套接字绑定到服务器地址上*/
	if(-1==bind( server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr) ))
	{
		perror("bind_faild");
		return 0;
	}
	/*按队列顺序监听套接字*/
	if( -1==listen(server_fd, 5) )
	{
		perror("listen");
		return 0;	
	}
	return conmmunication( server_fd);
}



bool conmmunication(int server_fd)
{

	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	while(1)
	{	
		printf("---------------wait for client---------------\n");
		sockConnect = accept(server_fd, (struct sockaddr *)&client_addr, &len);
		if(sockConnect<0)
		{
			perror("accept is failed");
			return 0;
		}
		uint8_t sendBuf[100]={0};
		printf("---------------client connected---------------\n");
		sprintf(sendBuf,"welcome client(%s) to server!",inet_ntoa(client_addr.sin_addr));
		
		ssize_t check = send(sockConnect, sendBuf, strlen(sendBuf)+1, 0);//发送字符串给客户端

		if(check == -1)
		{
			perror("send");
			close(sockConnect);
			return 0;
		}
		uint8_t recvbuf[BUFSIZ]={0};
		while(1)
		{	
			memset(recvbuf,0,BUFSIZ);
		#if N == 1
			ssize_t len = recv(sockConnect, recvbuf, sizeof(recvbuf), 0);
			if (len == -1) 
			{
				perror("recv");
				close(sockConnect);
				return 0;
			}
			else if(len == 0)
			{
				printf("client disconnected\n");
				close(sockConnect);
				break;
			}
			printf("recv: %s", recvbuf);
			
			// send(sockConnect, buf, len1, 0);
			
		#else
				memset(buf,0,BUFSIZ);
				ssize_t bytes_read = read(sockConnect,buf,BUFSIZ);
				if(bytes_read==-1){
					fprintf(stderr,"read ERROR:%s\n",strerror(errno));
					exit(EXIT_FAILURE);
				}
				else{
					printf("recv:%s",buf);
				}
		#endif
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
	if(sockConnect>0){
		close(sockConnect);
	}
	if(server_fd>0){
		close(server_fd);
	}

	printf("SIGINT\n");
	exit(EXIT_SUCCESS);
}