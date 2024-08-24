CC ?= gcc
CFLAGS = -Wall

target=multi_thread_server multi_thread_client

all: $(target)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

multi_thread_client: multi_thread_client.o
	$(CC) $(CFLAGS) -o $@ $< -lpthread
multi_thread_server: multi_thread_server.o
	$(CC) $(CFLAGS) -o $@ $< -lpthread
clean:
	rm -f $(target) *.o