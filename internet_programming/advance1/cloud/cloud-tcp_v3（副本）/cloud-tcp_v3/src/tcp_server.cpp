#include "tcp_server.h"
#include "mysocket.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

int sfd;

int main(int argc, char *argv[])
{
    int ret, max_fd;
    int rfd;
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));


    arg_check(argc, argv);
    my_socket::create_socket();
    sfd = my_socket::bind_socket(argv);
    my_socket::listen_socket();

    fd_set rfds,temp_rfds;
    FD_ZERO(&rfds);
    FD_ZERO(&temp_rfds);
    FD_SET(sfd, &rfds);
    max_fd = sfd;
    std::cout<<"waiting for client..."<<std::endl;
    while(1)
    {
        temp_rfds = rfds;
        ret = select(max_fd+1, &temp_rfds, nullptr, nullptr, nullptr);
        if(ret <0)
        {
            perror("select");
            exit(1);
        }
        if(FD_ISSET(sfd, &temp_rfds))//处理连接请求
        {
            socklen_t size_addr =sizeof(client_addr);
            rfd=accept(sfd,(struct sockaddr*)&client_addr,&size_addr);
           if(rfd <0)
           {
				perror("accept");
				continue;
           }
           
            std::cout << "New connection, socket fd: " << rfd
                      << ", IP: " << inet_ntoa(client_addr.sin_addr)
                      << ", PORT: " << ntohs(client_addr.sin_port) << std::endl;
            FD_SET(rfd, &rfds);
            if(rfd > max_fd)
            {
                max_fd = rfd;
            }
        }
	//处理所有客户端IO操作
        for(int i=sfd+1; i<=max_fd; i++)
        {
            if(FD_ISSET(i, &temp_rfds))
            {
            	handle_frame(i,rfds,temp_rfds, argv);
            }
        }

    }

}
