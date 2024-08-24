#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>


int main(int argc, char *argv[])
{
    int client_fd;
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));

      if (argc != 3)
    {
        perror("Usage: ./client <server_ip> <server_port>");
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
   
    char buf[1024];
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        printf("Please enter your message: ");
        fgets(buf, sizeof(buf), stdin);
        if(send(client_fd, buf, strlen(buf), 0)<0)
        {
            perror("send");
            close(client_fd);
            exit(EXIT_FAILURE);
        }

    }
}
