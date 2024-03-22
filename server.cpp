#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "base.h"
#include "Communication.h"
#include <time.h>


void check_player_state(player_ptr){
	socket = player_ptr->get_socket();
	//send the client the list of player name it invited
	std::vector<Player*> invited_list = player_ptr->get_invited_players();//TODO : implement get_invited_players
	std::string invited_players_name;
	for(int i=0;i< invited_list.size();i++){
		invited_players_name += invited_list[i]->get_name();
		invited_players_name += " ; ";//the delimiter between names
	}
	int name_list_size = invited_players_name.size();
	//send the name_list_size for the buffer to know the length
	size = write(socket, name_list_size, sizeof(name_list_size));// send the size of the string so the client can adapt buffer size
	if(size != sizeof(name_list_size));
	
	//send the list of invited players names
	const char* buffer = invited_players_name.c_str();
	size_t buffer_size = invited_players_name.size();
	ssize_t size;
	size = write(s, buffer, buffer_size);// send the size of the string so the client can adapt buffer size
	if(size != sizeof(buffer));
	
	
	
	int player_state = player_ptr->get_state();
	//sent client the state to tell it what to do next
	size = write(socket, player_state, sizeof(player_state));// send the size of the string so the client can adapt buffer size
	if(size != sizeof(player_state));
	
	
	if(player_state==0){/* On ne fait rien*/}
	while(player_state==1){//you are invited to a game
		
		std::vector<Player*> inviting_list = player_ptr->get_inviting_players();
		int player_number = inviting_list.size();
		size = write(socket, player_number, sizeof(player_number));
		if(size != sizeof(player_number));
		
		
		std::string inviting_players_name;
		for(int i=0;i< inviting_list.size();i++){
			inviting_players_name += std::to_string(i) += " : "
			inviting_players_name += inviting_list[i]->get_name();
			inviting_players_name += "\n";
		}
		int name_list_size = inviting_players_name.size();
		//send the name_list_size for the buffer to know the length
		size = write(socket, name_list_size, sizeof(name_list_size));// send the size of the string so the client can adapt buffer size
		if(size != sizeof(name_list_size));
		
		//send the list of inviting players names
		const char* buffer = inviting_players_name.c_str();
		size_t buffer_size = inviting_players_name.size();
		ssize_t size;
		size = write(s, buffer, buffer_size);// send the size of the string so the client can adapt buffer size
		if(size != sizeof(buffer));
		
		//ask (wait for answer) client if accept or reject invitation
		int players_choice = 0;
		size = read(f, players_choice, sizeof(players_choice));
		if(size != sizeof(players_choice));
		
		if(players_choice==-1){//cas de refus
			player_ptr->change_state(0);
			for(int i=0; i<inviting_list.size();i++){
				player_ptr->reject_invitation(inviting_list[i]);
			}
			player_state = player_ptr->get_state();
		}
		else if(players_choice>=0 || players_choice<player_number){//TODO : je m'étais arrêté là
			//invalidate other sent invitations (by me)
			//ask inviting to invalidate his other invitation
			//check if invitation is still valid
			//if valid :
				// sent the client that invitation valid (1)
				//change the state of this and inviting to 2
			//else //not valid
				//send client invitation not valid (0)
				//if no more invitation, change player_state to 0
			player_state = player_ptr->get_state();
		}
		else{
			// TODO : error case to manage
		}
	}
	// in the state was already 2, or just passed to 2
	player_state = player_ptr->get_state();
	if(player_state==2){
		//tell the client we enter the game (1)
		//enter the game
	
		}
	else{
		//tell the client we do not enter the game
	}
}

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
	
	Player* player_ptr = global_com.create_player(f, player_name); //state of player == 0
	// TO CHECK TODO : send to client waiting for another player
	std::string message = "Waiting for another player to connect to the server";
	const char* buffer = message.c_str();
	size_t buffer_size = message.size(); // should be max 50
	//ssize_t size;
	size = write(f, buffer, buffer_size);// send the size of the string so the client can adapt buffer size
	if(size != sizeof(buffer));
	
	while(1) {//s'exécute tant que je suis le seul joueur dispo sur le serveur
		int break_or_not = 0;
		player_list = global_com.get_player_list()
		if(player_list.size() <=1){/*just do nothing, wait*/}
		else{
			Player* other_player_ptr;
			// other player connected, check if some are available
			for(int i = 0; i<player_list.size();i++){
				//check if it is not us
				other_player_ptr = player_list[i];
				int player_socket = other_player_ptr->get_socket();
				if(player_socket == f){
					continue; //it is us
					}
				int player_state = player.get_state();
				//get the state
				//if available break the for and while loop
				if(player_state == 0 || player_state == 1){
					break_or_not = 1;
					}
							
			} 
			
		}
		if(break_or_not){break;}
		//wait some time
		
		struct timespec sleeping_time;
		sleeping_time.tv_sec = 0;
		sleeping_time.tv_nsec = 100000;
		nanosleep(&ts, nullptr);
		/*std::chrono::milliseconds timespan(100);
		std::this_thread::sleep_for(timespan);//TO CHECK TODO :voir avec le prof si on peut bien utiliser ça. */
	
	}//TODO : gérer le cas d'abandon d'un joueur
	
	//ici, il y a des joueurs connectés
	//check si ce joueur est invité:
	// dire au client message d'autres joueurs connectés (on entre dans les boucles de connexions)
	message.clear();
	buffer.clear();
	buffer_size.clear();
	message = "Other players have joined or are available again !";
	buffer = message.c_str();
	buffer_size = message.size(); // should be max 100
	//ssize_t size;
	size = write(f, buffer, buffer_size);// send the size of the string so the client can adapt buffer size
	if(size != sizeof(buffer));
	
	while(1){//dans cette boucle tant que on n'est pas dans un jeu
		
		//TODO call check_player_state(player_ptr)
		// demander si envoyer invitation(voir liste joueur) ou juste attendre
		//TODO call check_player_state(player_ptr)
		//if attendre : sleep(5); continue; (on revient au début
		//if envoyer invit:
			//envoyer liste noms joueurs dispo au client et demander choix 
			// recevoir la réponse du client
			//TODO call check_player_state(player_ptr)
			//envoyer l'invitation à autre joueur
			//envoyer au client le message que l'invitation a bien été envoyé
	}		
	
	
	
	/* début du bazar inutile
	std::vector<Player*> inviting_list = player_ptr->get_inviting_players();
	if(inviting_list.size()==0){
		//send to the client the list of connected players who are available
			///send 0 to the client, then
			///create and send a list of players names to client
			
			
		// the client get the choice of the human
		// 
		
		//has to wait and or invite other player
	}
	else if(inviting_list.size()>0){
		//has to choose to accept a game, or to wait, or to invite someone else
			///send 1 to the client, then
			///create a list of 
	}
	else{
		//there is a problem
		//TODO manage the problem
			///send -1 to the client
	}
	*/ //fin de bazar
	
	
		//envoyer liste des invitations
	
	//enter big waiting loop :
		
	
	// if invited list of player is empty :
		//send player list names; 
		// ask if it wants to invite or not
		// which one it invites
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
