#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "base.h"

int main (int argc, char * argv[])

{
        struct sockaddr_in saddr;
        struct hostent * server;
        int s, ret;
	//char * buf;

	if (argc == 1) {
		std::cerr << "usage: " << argv[0] 
			  << " [ adresse IP/nom du serveur ]" << std::endl;
		return 0;
	}

        s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s < 0) {
                std::cerr << "cannot create socket" << std::endl;
                return 0;
        }

        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(DEFAULT_PORT);
        server = gethostbyname(argv[1]);
        if (server == NULL) {
                std::cerr << "gethostbyname() failed" << std::endl;
		close(s);
                return 0;
        }
        else {
                memcpy(&saddr.sin_addr, server->h_addr, 4);
        }

        ret = connect(s, (struct sockaddr *)&saddr, sizeof(saddr));
        if (ret < 0) {
                std::cerr << "cannot connect to remote server" << std::endl;
                close(s);
                return 0;
        }

	//size_t len;
	//ssize_t size;
	// ask the name of the player
	std::cout << "connected to the server"<< std::endl <<"Please enter your name : ";
	std::string player_name = " ";
	
	while (!(std::cin >> player_name) || player_name == " " || player_name.size() >50 )
	{
		std::cout << "Invalid name, Please enter your name : " << std::endl;
        	std::cin.clear();
        	std::cin.ignore(1000, '\n');// TODO : gérer si la personne met plus de 1000 char dans la console
        }
	
	// write the length of the name to server
	// write the name to the server
	
	
	const char* buffer = player_name.c_str();
	size_t buffer_size = player_name.size(); // should be max 50
	ssize_t size;
	
	size = write(s, buffer, buffer_size);// send the size of the string so the client can adapt buffer size
	
	if(size != sizeof(buffer));
	std::cout << "your name has been sent to the server" << std::endl;
	buffer.clear();
	
	//receive the waiting message from server
	char buffer_message[100] = {0};
	size = read(f, buffer_message, sizeof(buffer_message));
	if(size != sizeof(buffer_message));
	std::string message(buffer_message);
	std::cout << message << std::endl;
	buffer_message.clear()
	
	int int_buff;
	size = read(f, int_buff, sizeof(int_buff));
	if(size != sizeof(int_buff));
	if(int_buff==0)//no one has invited this player
		{
		//need to receive the list of available player's names
		///print the name of available players with number
		/// ask if human want to invite another player
		/// ask the number of the player it want to invite
		///send to server the 
		}
	else if(int_buff==1)//this player has been invited by others
		{
		//need to receive the list of available player's names
		//and the list of inviting player names
		}
	else if(int_buff==-1)//there is a problem on the server : inviting_list.size()
		{
		std::cout <<"there is a problem on the server with the value of inviting_list.size()" << tsd::endl;
		//TODO : decide what to do
		}
	else{} //there is a problem in the communication
	
	int_buff.clear()
	
	
	//size = write(f, player_name, 1 + strlen(player_name));
	
	//if(size != sizeof(len_name));
	
	
	/*
	size = read(s, &len, sizeof(len));
	if(size != sizeof(len));
	
	buf = new char [len + 1]; //why not sore it in a string directly ? 
	size = read(s, buf, 1 + len);
	if(size != sizeof(len));
	
	std::cout << buf << std::endl;
	*/
	close(s);
	//delete [] buf;
	return 0;
}
