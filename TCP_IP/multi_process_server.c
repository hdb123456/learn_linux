#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

int init(int server_fd);
void recv_data(int server_fd,int client_fd);
void check(const char* message, int server_fd,int client_fd);
//#ifdef N
#define N 1
//#endif

int main (int argc,char *argv[])
{	int server_fd = -1,client_fd = -1;
	server_fd = init(server_fd);
	recv_data( server_fd, client_fd );
	return 0;
}
int init(int server_fd)
{	
	struct sockaddr_in server_addr; 
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd<0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	/*将套接字绑定到服务器地址上*/
	if( bind( server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr) )<0 )
	{
		check("bind", server_fd, -1);//错误处理,绑定失败关闭接口
	}
	if( listen(server_fd, 5)<0 )
	{
		check("listen", server_fd, -1);
	}
	return	server_fd;
}


/*
*  @brief 
*  @param  
*/ 
void recv_data(int server_fd,int client_fd)
{
	struct sockaddr_in client_addr;
	
	if (server_fd < 0) 
	{
		perror("Invalid server_fd");
		exit(EXIT_FAILURE);
	}
	socklen_t len = sizeof(client_addr);
	client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
	if(client_fd<0)
	{
		check("accept", server_fd, client_fd);
	}
	printf("client connect\n");
	//char buf[BUFSIZ]={0};
	char buf[1024] = {0};
	while(1)
	{	
#if N == 1
    int len1 = recv(client_fd, buf, sizeof(buf), 0);
    if (len1 < 0) {
        check("recv", server_fd, client_fd);
    } else {
        printf("recv: %s\n", buf);
        // send(client_fd, buf, len1, 0);
    }
#else
		memset(buf,0,BUFSIZ);
		ssize_t bytes_read = read(client_fd,buf,BUFSIZ);
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


void check(const char *message, int server_fd, int client_fd)
{
	perror(message);
	if(server_fd>=0)
		close(server_fd);
	if(client_fd>=0)
		close(client_fd);
	exit(EXIT_FAILURE);
}