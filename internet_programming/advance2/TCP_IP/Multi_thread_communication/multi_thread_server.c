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
#include <sys/wait.h>
#include <pthread.h>

uint16_t sockConnect=-1;
uint8_t server_fd = -1;

void Call_handle(void);
void handle_SIGINT(int sig);

int init(int server_fd, char* argv[]);
bool conmmunication(int server_fd);
void *client_handle(void* client_addr);

//#ifdef N
#define N 1
//#endif

int main (int argc,char *argv[])
{	
    if(argc!=3)
	{
		printf("Usage: ./server ip port\n");
		exit(EXIT_FAILURE);
	}
	Call_handle();
	bool flag = init(server_fd,argv);
	if(flag==0)
	{
		perror("init");
		close(server_fd);
		exit(EXIT_FAILURE);
	}
	return 0;
}
int init(int server_fd, char* argv[])
{	
	struct sockaddr_in server_addr;
	int on = 1;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd<0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	/*地址快速重用*/
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))==-1)
	{
		perror("setsockopt");
		return 0;
	}
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	/* 设置服务器地址结构体的成员 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));//htons(8080)
	#if N==1 
		if ( inet_aton(argv[1], &server_addr.sin_addr)==0)
			{
				perror("inet_aton");
				return 0;
			}
	#else
			server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(argv[1])
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
	pthread_t tid;
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	while(1)
	{	
		sockConnect = accept(server_fd, (struct sockaddr *)&client_addr, &len);
		if(sockConnect<0)
		{
			perror("accept is failed");
			return 0;
		}
		char sendBuf[100]={0};
		printf("---------------client ip(%s) port(%d) to server---------------\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		sprintf(sendBuf,"welcome client ip(%s) port(%d) to server!",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		ssize_t check = send(sockConnect, sendBuf, strlen(sendBuf)+1, 0);//发送字符串给客户端

		if(check == -1)
		{
			perror("send");
			close(sockConnect);
			return 0;
		}
        pthread_create(&tid,NULL,client_handle,(void*)&client_addr);
        pthread_detach(tid);
	}
}

void *client_handle(void* client_addr_ptr)
{
    struct sockaddr_in client_addr = *(struct sockaddr_in*)client_addr_ptr;
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
            break;
        }
        else if(len == 0)
        {
            printf("---------------client ip(%s) port(%d) disconnected!---------------\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
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
    pthread_exit(NULL);
}

void Call_handle(void)
{
	struct sigaction act1;
	act1.sa_handler = handle_SIGINT;
	act1.sa_flags = 0;
	sigemptyset(&act1.sa_mask);
	sigaction(SIGINT, &act1, NULL);
}

void handle_SIGINT(int sig){
	if(sockConnect>0)
    {
		close(sockConnect);
	}
	if(server_fd>0)
    {
		close(server_fd);
	}
	exit(EXIT_SUCCESS);
}