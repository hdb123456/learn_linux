CC = gcc
CFLAGS = -Wall

target = unix_server unix_client

all: $(target)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

unix_server: unix_server.o
	$(CC) $(CFLAGS) -o $@ $<

unix_client: unix_client.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(target) *.o ~/unix/temp
