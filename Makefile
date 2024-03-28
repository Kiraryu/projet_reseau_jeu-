CC = g++
CFLAGS = -Wall -Wshadow -Werror -O

all: client server

client: client.o
	$(CC) $(CFLAGS) -o client client.o

server: server.o
	$(CC) $(CFLAGS) -o server server.o

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp -o client.o

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp -o server.o

clean:
	rm -f client server *.o

