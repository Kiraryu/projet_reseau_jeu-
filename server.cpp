#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "base.h"
#include "Communication.h"

void * hconnect (void * fd)

{
	int f = *((int *)fd);// get socket number

	// regsiter the new player in the communication instance
	// get the name of the player :
		// read the name size
	//size_t len_name;
	ssize_t size;
	//size = read(s, &len_name, sizeof(len_name));// get the name len to define size of buffer
	char buffer[50] = {0};
	size = read(f, buffer, sizeof(buffer));
	if(size != sizeof(buffer));
	std::string player_name(buffer);
	std::cout << "player_name : " << player_name << "on socket : " << f << std::endl;
	
	global_com.create_player(f, player_name); //state of player == 0
	while(1) {
		player_list = global_com.get_player_list()
		if(global_com.
	
	}
	
	
	
	//enter big waiting loop :
		// wiating while the player is alone on server
	
	// if invited list of player is empty :
		
	// else : the player has been invited :
		//give choice, join the game, or invite another player
	
		
	//loop waiting for another player
		// print the list of other player
		// give the choice to invite another player, while waiting for an invitation
	
	//when agreed with another player -start a game:
	//create and register the new game
	//enter the game loop	


	/*
	char tmp[100];
	time_t t;
	struct tm * T;

	time(&t);
	T = localtime(&t);
	snprintf(tmp, sizeof(tmp), "%s", asctime(T));



	size_t len = strlen(tmp);
	ssize_t size;
	size = write(f, &len, sizeof(len));// send the size of the string so the client can adapt buffer size
	if(size != sizeof(len));
	size = write(f, tmp, 1 + strlen(tmp));
	if(size != sizeof(len));
	*/
	close(f); //close the socket

	free(fd); //free the memory of socket pointer
	pthread_detach(pthread_self());
	return NULL;	
}

int main (int argc, char ** argv)

{
        const char * server = "0.0.0.0"; // convention pour se mettre en écoute sur toutes les cartes réseaux.
        struct sockaddr_in sin;
        int s, f, ret;
	pthread_t tid;
	int optval = 1;

        sin.sin_family = AF_INET; //permet de dire qu'on est en IPv4
        sin.sin_port = htons(DEFAULT_PORT); // encodage du port, htons : host to network short
        ret = inet_aton(server, &sin.sin_addr);
        if (ret == 0) {
		fprintf(stderr, "address %s invalid\n", server);
		return 0;
	}

        s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s < 0) {
		fprintf(stderr, "cannot create socket\n");
		return 0;
	}

        ret = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        if (ret != 0) {
                fprintf(stderr, "setsockopt() failed\n");
                return 0;
        }
        
	//bind : fonction pour relier les cartes réseaux aux sockets
        ret = bind(s, (struct sockaddr *)&sin, sizeof(sin));
        if (ret != 0) {
		fprintf(stderr, "bind() failed\n");
		return 0;
	}
	
	// listen : se mettre en attente des connexion entrantes
	// listen(s, 10) : max 10 connnexions en même temps 
        ret = listen(s, 10);
        if (ret != 0) {
		fprintf(stderr, "listen() failed\n");
		return 0;
	}
	// instanciate a gloabl class communication
	Communication global_com;
	
	
        while (1) {
        	//int accept(int socket, struct sockaddr *restrict address,socklen_t *restrict address_len);
        	// https://pubs.opengroup.org/onlinepubs/009696699/functions/accept.html
                f = accept(s, NULL, 0);
                if (f == -1) {
			fprintf(stderr, "accept() failed\n");
			return 0;
		}

		int * fd = new int;
		*fd = f;
		pthread_create(&tid, NULL, hconnect, (void *)fd);
        }

        return 0;
}
