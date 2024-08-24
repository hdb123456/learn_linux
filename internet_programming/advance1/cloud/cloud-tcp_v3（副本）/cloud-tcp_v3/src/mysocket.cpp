#include "mysocket.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
//#include <stdio.h>
#define FILE_NAME_SIZE 256
#define BUFFER_SIZE 1024
#define LISTEN_NUM 5
#define handle_error(msg)                                                      \
    do                                                                         \
    {                                                                          \
        std::cerr << msg << std::endl;                                         \
        exit(1);                                                               \
    } while (0)



void arg_check(int argc, char *argv[])
{
    if (argc != 4)
    {
        handle_error("Usage: ./client <server_ip> <server_port>");
        exit(1);
    }
}

void my_socket::create_socket(void)
{
	int on=1;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        handle_error("Error creating socket");
        exit(1);
    }
    /*地址快速重用*/
	if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))==-1)
	{
		perror("setsockopt");
	}
	std::cout<<"socket:"<<sfd<<std::endl;
}
int my_socket::bind_socket(char *argv[])
{
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[2]));
    //saddr.sin_addr.s_addr = inet_addr(argv[1]);
    if(inet_aton(argv[1], &saddr.sin_addr));
    std::cout<<"bind_socket:"<<sfd<<std::endl;
    if (bind(sfd, (struct sockaddr *)&saddr, (socklen_t)sizeof(struct sockaddr_in)) < 0)
    {
        handle_error("Error binding socket");
        exit(1);
    }
    return sfd;
}
void my_socket::listen_socket(void)
{
    if(listen(sfd, LISTEN_NUM) == -1)
    {
        handle_error("Error listening");
    }
}
//从文件描述符中获取客户端信息
void get_peer(int fd)
{
	struct sockaddr_in addr;
	socklen_t size = sizeof(addr);
	getpeername(fd, (struct sockaddr*)&addr, &size);
	std::cout << "ip:"<< inet_ntoa(addr.sin_addr) 
              << ", PORT: " << ntohs(addr.sin_port) << std::endl;
}
int mkdir_filename(char*filename)
{
	int fd = open(filename, O_CREAT|O_RDWR, 0777);
	if(fd == -1)
	{
		handle_error("create or open");
	}
	return fd;
}

void handle_frame(int new_socket, fd_set &rfds, fd_set &temp_rfds, char *argv[])
{
    char file_name[FILE_NAME_SIZE] = {0};
    uint32_t file_name_length;
    ssize_t len = recv(new_socket, &file_name_length, sizeof(file_name_length), 0);
    if (len <= 0) {
        perror("recv file name length");
        std::cout << "Client on socket " << new_socket << " disconnected." << std::endl;
        FD_CLR(new_socket, &rfds);
        FD_CLR(new_socket, &temp_rfds);
        exit(EXIT_FAILURE);
    }
    file_name_length = ntohl(file_name_length);
    
    ssize_t received = 0;
    while (received < file_name_length) {
        len = recv(new_socket, file_name + received, file_name_length - received, 0);
        if (len <= 0) {
            perror("recv file name");
            std::cout << "Client on socket " << new_socket << " disconnected." << std::endl;
            FD_CLR(new_socket, &rfds);
            FD_CLR(new_socket, &temp_rfds);
            exit(EXIT_FAILURE);
        }
        received += len;
    }
    file_name[file_name_length] = '\0'; // Ensure string termination
    std::cout << "file_name:" << file_name << std::endl;
    
    // Define the directory path
    const char *directory_path = argv[3];

    // Create a new string with the full path
    char full_path[FILE_NAME_SIZE + 256]; // Adjust size as needed
    snprintf(full_path, sizeof(full_path), "%s%s", directory_path, file_name);
    
    std::cout << "full_path:" << full_path << std::endl;
    
    // Open file with the full path
    FILE* fp = fopen(full_path, "wb");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    char buffer[BUFFER_SIZE] = {0};
    const char end_marker[] = "END_OF_FILE";
    ssize_t end_marker_len = sizeof(end_marker) - 1;
    
    ssize_t total_received = 0;
    while (true) {
        len = read(new_socket, buffer, BUFFER_SIZE);
        if (len > 0) {
            if (len >= end_marker_len && memcmp(buffer + len - end_marker_len, end_marker, end_marker_len) == 0) {
                // Only write data before the end marker
                fwrite(buffer, sizeof(char), len - end_marker_len, fp);
                break;
            } else {
                fwrite(buffer, sizeof(char), len, fp);
            }
            total_received += len;
        } else if (len == 0) {
            // Connection closed
            break;
        } else {
            // Error
            perror("read file");
            break;
        }
        memset(buffer, 0, BUFFER_SIZE);
    }
    fclose(fp);
    std::cout << "end of file transfer" << std::endl;
}

 


