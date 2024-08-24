#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include "client_base.h"

#define BUFFER_SIZE 1024
#define FILE_NAME_SIZE 256

int main(int argc, char *argv[])
{
    //socket
    int client_fd;
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    //data
    FILE *fp;
    char buffer[BUFFER_SIZE]={0};
    char file_name[FILE_NAME_SIZE];
    if (argc != 4)
    {
        perror("Usage: ./client <server_ip> <server_port> <filename>");
        exit(1);
    }
   
    
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(atoi(argv[2]));
    client_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if ( connect(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
    {
        perror("Error connect socket");
        exit(1);
    }
     
    while(1)
    {
        printf("Please press `enter` in the keyboard or `q` to cancel\n");
        bool flag =login_input();
        if(flag==0)
        {
            break;
        }
        else
        {
        	memset(buffer, 0, sizeof(buffer));
			// 获取文件名长度
			snprintf(file_name, FILE_NAME_SIZE, "%s", argv[3]);
			uint32_t file_name_length = strlen(file_name);
			file_name_length = htonl(file_name_length);
			if(send(client_fd, &file_name_length, sizeof(file_name_length), 0)<0){
				perror("send file_name_head");
				close(client_fd);
				exit(EXIT_FAILURE);
			}
			std::cout << "file_name_length" << file_name_length << std::endl;
			//发送文件名
			if(send(client_fd, file_name, strlen(file_name), 0)<0)
			{
				perror("send file_name");
				close(client_fd);
				exit(EXIT_FAILURE);
			}
			std::cout << "file_name" << file_name << std::endl;
			//打开文件并读取
			if ((fp = fopen(argv[3], "rb")) == NULL) {
				perror("fopen");
			}
			// 发送文件内容
			ssize_t bytes_read;
			while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
				send(client_fd, buffer, bytes_read, 0);
			}

			//结束
			  const char end_marker[] = "END_OF_FILE";
			if(send(client_fd, end_marker, strlen(end_marker), 0)<0)
			{
				perror("send file_name");
				close(client_fd);
				exit(EXIT_FAILURE);
			}
			fclose(fp);
        }
        
    }
    close(client_fd);
    return 0;
}
