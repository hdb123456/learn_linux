#ifndef MYSOCKET_H
#define MYSOCKET_H

extern int sfd;

class my_socket{
	public:
	static void create_socket(void);
	static int bind_socket(char *argv[]);
	static void listen_socket(void);
};

void arg_check(int argc, char *argv[]);
void get_peer(int fd);

#endif
