CC = g++

CFLAGS = -Wall -O2
LDFLAGS = -lsqlite3

OBJECTS = base.o
TARGET = sqlite3

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(TARGET)


