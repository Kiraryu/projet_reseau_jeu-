CLIENT = client.o
SERVER = server.o Communication.o Player.o param.o

all: client server

server: $(SERVER)
	c++ -o server $(SERVER) -lpthread

client: $(CLIENT)
	c++ -o client $(CLIENT)

-include $(CLIENT:.o=.d)
-include $(SERVER:.o=.d)

%.o: %.cpp
	c++ -c $*.cpp -o $*.o -Wall -Wshadow -Werror -O
	c++ -MM $*.cpp > $*.d
