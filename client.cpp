#include <unistd.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "base.h"

int communicate_check_player_state(int socket){
	
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
	
	// write the length of the name to servercommunicate_check_player_state
	// write the name to the server
	
	
	const char* buffer = player_name.c_str();
	size_t buffer_size = player_name.size(); // should be max 50
	ssize_t size;
	
	size = write(s, buffer, buffer_size);// send the size of the string so the client can adapt buffer size
	
	if(size != sizeof(buffer));
	std::cout << "your name has been sent to the server" << std::endl;
	
	
	//receive the waiting message from server
	// TODO : use a function receive and cout a message (less than 100 char)

	
	//receive the message that someone has joined server
	
	
	
	//enter the connexion loop
	
	
	int starting_game = 0;
	
	while(1){//boucle de connexion, normalement achevée
		// communication avec check_player_state
		std::cout << "entering connexion loop "<< std::endl;
		starting_game = communicate_check_player_state(s);
		if(starting_game==2){
			//We entered the game
			break;
		}
		// choisir envoyer invitation (donc voir liste) ou juste attendre
		//receive player list from server
		int number_available_players;
		size = read(s, &number_available_players, sizeof(number_available_players));
		if(size != sizeof(number_available_players));
		//std::cout << "number_available_players : " << number_available_players;
		int buffer_size4 = 0;
		size = read(s, &buffer_size4, sizeof(buffer_size4));
		if(size != sizeof(buffer_size4));
		
		char* buffer4 = new char[buffer_size4];
		size = read(s, buffer4, sizeof(buffer4));
		if(size != sizeof(buffer4));
		std::string available_players_name(buffer4);
		std::cout << "The following players are available on the server : " << std::endl;
		std::cout << available_players_name << std::endl;
		std::cout << "To send an invitation enter the number, to wait 5 sec enter 'w'."<< std::endl;
		
		std::string player_choice = " ";
		int int_player_choice;
		bool invalid_player_choice = true;
		//répondre avec un numéro ou "w" comme dans communicate_check_player_state
		while (!(std::cin >> player_choice) || invalid_player_choice)
		{	
			if(player_choice=="w"){
				invalid_player_choice= false;
				int_player_choice = -1;// cas de refus d'invitation
				continue;
			}
			//essayer de le convertir en int, si marche pas continue
			else{
				
				try {
					int_player_choice = stoi(player_choice);
					if(int_player_choice>=0 && int_player_choice<number_available_players){
						invalid_player_choice= false;
						//cas d'acceptation d'invitation
					}
					else{
						std::cout << "Invalid choice, Please enter a correct number to send an invitation or 'w' to wait " << std::endl;
					}
				}
				catch(...){
					std::cout << "Invalid choice, Please enter a correct number to send an invitation or 'w' to wait " << std::endl;
				}
			}
			std::cin.clear();
			std::cin.ignore(1000, '\n');// TODO : gérer si la personne met plus de 1000 char dans la console
		}
		
		
		
		
		// envoyer la réponse au serveur
		size = write(s, &int_player_choice, sizeof(int_player_choice));
		if(size != sizeof(int_player_choice));
		//if attendre :
			//afficher qu'on est en attente, revenir au début //l'attente est gérée côté serveur, on attend qu'il reprenne la communication avec check_player_state
		if(int_player_choice==-1){
			std::cout << "You chose to wait, so wait 5 seconds" << std::endl;
		}
		//else if envoyer invit:
			//recevoir le message de confirmation de l'envoie de l'invitation, retour au début
		else if(int_player_choice>=0 && int_player_choice<number_available_players){
			char* message_buffer = new char[100];
			size = read(s, message_buffer, sizeof(message_buffer));
			if(size != sizeof(message_buffer));
			message = message_buffer;
			std::cout<< message << std::endl;
		}
		
		// communication avec check_player_state
		starting_game = communicate_check_player_state(s);
		if(starting_game==2){
			//We entered the game
			std::cout << "Starting Game !" << std::endl;
			break;
		}
		
		
	}
	//TODO : entrer dans la boucle de jeu, en parallèle de serveur
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
