#include <unistd.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "base.h"

void communicate_check_player_state(int socket){
	//receive the name of players in pending invitation
	int buffer_size = 0;
	size = read(f, buffer_size, sizeof(buffer_size));
	if(size != sizeof(buffer_size));
	
	char* buffer = new char[buffer_size];
	size = read(f, buffer, sizeof(buffer));
	if(size != sizeof(buffer));
	std::string invited_player_name(buffer);
	
	std::cout<< "The list player you invited, that have not already rejected your invitation : " << std::endl;
	std::cout << invited_player_name << std::endl;
	

	//receive an int that give the state and will influence the next actions
	int player_state = -1;
	size = read(f, player_state, sizeof(player_state));
	if(size != sizeof(player_state));
	
	if(player_state==0){/* On ne fait rien*/}
	else if(player_state==-1){
		std::cout<< "communication with server seemed to fail in communicate_check_player_state" << std::endl;
	}
	else if(player_state==2){
		// wait for further communication to say we enter the game
	}
	else{
		//TODO : manage problem
	}
	while(player_state==1){
		//receive the number of inviting players
		int players_number = 0;
		size = read(f, players_number, sizeof(players_number));
		if(size != sizeof(players_number));
		
		//receive the names of inviting players
		int buffer_size = 0;
		size = read(f, buffer_size, sizeof(buffer_size));
		if(size != sizeof(buffer_size));
		
		
		char* buffer = new char[buffer_size];
		size = read(f, buffer, sizeof(buffer));
		if(size != sizeof(buffer));
		std::string inviting_players_name(buffer);
		std::cout << "The following players invited you : " << std::endl;
		std::cout << inviting_players_name;
		std::cout << "To accept an invitation enter the number, to refuse enter 'n'."
		
		std::string player_choice = " ";
		int int_player_choice;
		bool invalid_player_choice = true;
		while (!(std::cin >> player_choice) || invalid_player_choice)
		{	
			if(player_choice=="n"){
				invalid_player_choice= false;
				int_player_choice = -1;// cas de refus d'invitation
				continue;
			}
			//essayer de le convertir en int, si marche pas continue
			else{
				int choice
				try {
					int_player_choice = stoi(player_choice);
					if(int_player_choice>=0 || int_player_choice<players_number){
						invalid_player_choice= false;
						//cas d'acceptation d'invitation
					}
				}
				catch(...){
					std::cout << "Invalid choice, Please enter a correct number to accept or 'n' to refuse " << std::endl;
				}
			}
			std::cin.clear();
			std::cin.(1000, '\n');// TODO : gérer si la personne met plus de 1000 char dans la console
		}
		//send answer to server
		size = write(socket, int_player_choice, sizeof(int_player_choice));
		if(size != sizeof(int_player_choice));
		//TODO : je m'étais arrêté là
		
		
		//if accepted :
			//receive invitation valid or not
			//if 1 (valid)
				//do nothing, the game will enter after
			//else (not valid)
				//print that invitation not valid anymore
		else{	
			//afficher qu'on a refusé les invitations 
		}
	}
	int game_state = -1;
	size = read(f, player_state, sizeof(game_state));
	if(size != sizeof(game_state));
	if(game_state==1){
		//afficher We enter the game
	}
	else{
		// afficher qu'on retourne à ce à quoi on était
	}
	
	
	
} 


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
	
	//receive the message that someone has joined server
	size = read(f, buffer_message, sizeof(buffer_message));
	if(size != sizeof(buffer_message));
	std::string message(buffer_message);
	std::cout << message << std::endl;
	buffer_message.clear()
	//enter the connexion loop
	
	while(1){
		//TODO communication avec check_player_state
		
		// choisir envoyer invitation (donc voir liste) ou juste attendre
		// envoyer la réponse au serveur
		
		//TODO communication avec check_player_state
		
		//if attendre :
			//afficher qu'on est en attente, revenir au début //l'attente est gérée côté serveur, on attend qu'il reprenne la communication avec check_player_state
		//if envoyer invit:
			//recevoir la liste des noms des joueurs dispo, afficher
			// demander le choix du joueur
			// envoyer la réponse au serveur
	
			//TODO communication avec check_player_state
			
			//recevoir le message de confirmation de l'envoie de l'invitation, retour au début
	
	}
	
	/* début bazar inutile
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
	*///fin bazar
	
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
