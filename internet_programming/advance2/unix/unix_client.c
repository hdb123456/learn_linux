#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MY_SOCK_PATH "/home/mingze/unix/temp"

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define len 1024

int main(void)
{
	int cfd;
	char buff[len]={0};
	struct sockaddr_un peer_addr;

	cfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (cfd == -1)
		handle_error("socket");

	memset(&peer_addr, 0, sizeof(peer_addr));
	peer_addr.sun_family = AF_UNIX;
	strncpy(peer_addr.sun_path, MY_SOCK_PATH, sizeof(peer_addr.sun_path) - 1);

	if (connect(cfd, (struct sockaddr *) &peer_addr, sizeof(peer_addr)) == -1)
		handle_error("bind");
	printf("input data:");
	fgets(buff,sizeof(buff),stdin);
	send(cfd,buff,strlen(buff),0);
	/* Code to deal with incoming connection(s)... */

	if (close(cfd) == -1)
		handle_error("close");
	return 0;
}

