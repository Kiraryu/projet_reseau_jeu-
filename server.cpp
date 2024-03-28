#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "base.h"

#include <time.h>
#include <vector>
#include "param.h" //the class to communicate needed parameters to threads
#include <semaphore.h>

// Define a semaphore for synchronization
sem_t global_com_sem;

int check_player_state(Player* player_ptr, int socket){
	Player* partner_player_ptr = nullptr;//to be shure it is intialized
	socket = player_ptr->get_socket();
	//TODO : do it only in the case player state==1
	//send the client the list of player name it invited
	std::vector<Player*> invited_list = player_ptr->get_invited_players();
	std::string invited_players_name;
	for(int i=0;i< (int)invited_list.size();i++){
		invited_players_name += invited_list[i]->get_name();
		invited_players_name += " ; ";//the delimiter between names
	}
	int name_list_size = invited_players_name.size();
	//send the name_list_size for the buffer to know the length
	char* int_buffer = new char[10];// a size of 1 should be enough, but in case, 10, trying to avoid errors
	int_buffer[0] = name_list_size;//fonctionne tant que inférieur à 122
	ssize_t size;
	size = write(socket, int_buffer, sizeof(int_buffer));// send the size of the string so the client can adapt buffer size
	if(size != sizeof(int_buffer));
	
	//send the list of invited players names
	const char* buffer = invited_players_name.c_str();
	size_t buffer_size = invited_players_name.size();
	
	size = write(socket, buffer, buffer_size);// send the size of the string so the client can adapt buffer size
	if(size != sizeof(buffer));
	
	
	
	int player_state = player_ptr->get_state();
	//sent client the state to tell it what to do next
	int_buffer[0] = player_state;
	size = write(socket, int_buffer, sizeof(int_buffer));// send the size of the string so the client can adapt buffer size
	if(size != sizeof(int_buffer));
	
	
	if(player_state==0){/* On ne fait rien*/}
	while(player_state==1){//you are invited to a game
		
		std::vector<Player*> inviting_list = player_ptr->get_inviting_players();
		int player_number = inviting_list.size();
		int_buffer[0] = player_number;
		size = write(socket, int_buffer, sizeof(int_buffer));
		if(size != sizeof(int_buffer));
		
		
		std::string inviting_players_name;
		for(int i=0;i< (int)inviting_list.size();i++){
			inviting_players_name += std::to_string(i) += " : ";
			inviting_players_name += inviting_list[i]->get_name();
			inviting_players_name += "\n";
		}
		int name_list_size2 = inviting_players_name.size();
		//send the name_list_size for the buffer to know the length
		int_buffer[0] = name_list_size2;
		size = write(socket, int_buffer, sizeof(int_buffer));// send the size of the string so the client can adapt buffer size
		if(size != sizeof(int_buffer));
		
		//send the list of inviting players names
		const char* buffer2 = inviting_players_name.c_str();
		size_t buffer_size2 = inviting_players_name.size();
		//ssize_t size;
		size = write(socket, buffer2, buffer_size2);// send the size of the string so the client can adapt buffer size
		if(size != sizeof(buffer2));
		
		//ask (wait for answer) client if accept or reject invitation
		int players_choice = 0;
		size = read(socket, int_buffer, sizeof(int_buffer));
		if(size != sizeof(int_buffer));
		players_choice = int_buffer[0];
		
		if(players_choice==-1){//cas de refus
			player_ptr->change_state(0);
			for(int i=0; i< (int)inviting_list.size();i++){
				player_ptr->reject_invitation(inviting_list[i]);
			}
			player_state = player_ptr->get_state();
		}
		else if(players_choice>=0 && players_choice<player_number){
			//get the pointer of the inviting player
			partner_player_ptr = inviting_list[players_choice]; 
			
			//invalidate other sent invitations (by me)
			for(int i=0; i< (int)invited_list.size();i++){
				player_ptr->invalidate_invitation(invited_list[i]);
			}
			//ask inviting to invalidate his other invitation
			std::vector<Player*> partner_invited_list = partner_player_ptr->get_invited_players();
			bool invitation_still_valid = false;
			
			for(int i=0; i< (int)partner_invited_list.size();i++){
				if(partner_invited_list[i]==player_ptr){//invitation is still valid
					invitation_still_valid=true;
				}
				partner_player_ptr->invalidate_invitation(partner_invited_list[i]);
			}
			
			// sent to the client if the invitation is valid or not
			int invitation_valid = invitation_still_valid;
			size = write(socket, &invitation_valid, sizeof(invitation_valid));
			if(size != sizeof(invitation_valid));
		
			
			//check if invitation is still valid
			if(invitation_still_valid){
				//reject all other received invitation, and ask partner to do so;
				for(int i=0; i<(int)inviting_list.size();i++){
					player_ptr->reject_invitation(inviting_list[i]);
				}
				std::vector<Player*> partner_inviting_list = partner_player_ptr->get_inviting_players();
				
				for(int i=0; i<(int)partner_inviting_list.size();i++){
					partner_player_ptr->reject_invitation(partner_inviting_list[i]);
				}
				// send name of the partner player to client
				std::string partner_name = partner_player_ptr->get_name();//should be less than 50 char
				const char* buffer3 = partner_name.c_str();
				size_t buffer_size3 = partner_name.size();//50 max
				//ssize_t size;
				size = write(socket, buffer3, buffer_size3);// send the size of the string so the client can adapt buffer size
				if(size != sizeof(buffer3));
				//change the state of this and inviting to 2
				partner_player_ptr->change_state(2);
				player_ptr->change_state(2);
				
			}
				
			else{ //not valid
				inviting_list = player_ptr->get_inviting_players();
				//if no more invitation, change player_state to 0
				if(inviting_list.size()<1){
					//no more invitation in this case.
					player_ptr->change_state(0);
				}
				//else do nothing, the loop will ask to choose again
			}
			//update player_state in case its value has changed to 0 or 2
			player_state = player_ptr->get_state();
			//send the new player state to client to enable it to go out of loop
			size = write(socket, &player_state, sizeof(player_state));
			if(size != sizeof(player_state));
		}
		else{
			std::cout << "error case" << std::endl;
			
		}
	}
	// in the state was already 2, or just passed to 2
	player_state = player_ptr->get_state();
	size = write(socket, &player_state, sizeof(player_state));
	if(size != sizeof(player_state));
	if(player_state==2){
		
		// afficher log début jeu entre 2 joueur 
		std::cout << "log : a game is starting between ";
		std::cout << player_ptr->get_name() << " and " << partner_player_ptr->get_name() << std::endl;
		// TODO : appeler la fonction d'entrer dans le jeu qui permet de créer une nouvelle partie pour stocker les infos : quel joueur avec quel joueur. 
		
		//enter the game
		return 2;
	
		}
	else{	
		return 0;
	}
}

void * hconnect (void * thread_param_ptr)

{	
	Param thread_param = *((Param*) thread_param_ptr);
	// NO DO NOT create a copy : Communication global_com(*(thread_param.m_global_com_ptr));//doneTODO : créer constructeur de recopie
        // INSTEAD use the pointer : 
        Communication *global_com_ptr = thread_param.m_global_com_ptr; // Get pointer to global_com
	
	
	int f = *(thread_param.m_socket_ptr);// get socket number
	
	// regsiter the new player in the communication instance
	// get the name of the player :
	ssize_t size;
	//size = read(s, &len_name, sizeof(len_name));// get the name len to define size of buffer
	char buffer4[50] = {0};
	size = read(f, buffer4, sizeof(buffer4));
	if(size != sizeof(buffer4));
	std::string player_name(buffer4);
	std::cout << "player_name : " << player_name << " on socket : " << f << std::endl;
	
	// Critical section: Access and modify global_com 
	sem_wait(&global_com_sem); // Acquire semaphore lock

    	// Create player within the critical section
    	Player *player_ptr = global_com_ptr->create_player(f, player_name); //state of player == 0

    	sem_post(&global_com_sem); // Release semaphore lock
    	
    	// TO CHECK TODO : send to client waiting for another player
	
	std::string message = "Waiting for another player to connect to the server";
	const char* buffer5 = message.c_str();
	size_t buffer_size5 = message.size(); // should be max 50
	//ssize_t size;
	size = write(f, buffer5, buffer_size5);// send the size of the string so the client can adapt buffer size
	if(size != sizeof(buffer5));
	sem_wait(&global_com_sem);
	std::vector<Player*> player_list = global_com_ptr->get_player_list();
	sem_post(&global_com_sem);
	int counter = 0;
	while(1) {//s'exécute tant que je suis le seul joueur dispo sur le serveur
		int break_or_not = 0;
		// Critical section: Access global_com 
		sem_wait(&global_com_sem);
		player_list = global_com_ptr->get_player_list();
		sem_post(&global_com_sem);
		if(player_list.size() <=1){
			/*just do nothing, wait*/
			if(counter==10000){
				counter=0;
			}
			if(counter==0){
				std::cout << "Only one player on the server" << std::endl;
				std::string player_names;
				for(int i=0; i< (int)player_list.size() ;i++){
					player_names+= player_list[i]->get_name();
					player_names += " ; ";
				}
				std::cout << "Player names on socket " << f <<std::endl;
				std::cout << player_names << std::endl;
			}
			counter+=1;
			
			
		}	
		else{
			Player* other_player_ptr;
			// other player connected, check if some are available
			for(int i = 0; i<(int)player_list.size();i++){
				//check if it is not us
				other_player_ptr = player_list[i];
				int player_socket = other_player_ptr->get_socket();
				if(player_socket == f){
					continue; //it is us
					}
				int player_state = player_ptr->get_state();
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
		nanosleep(&sleeping_time, nullptr);
		/*std::chrono::milliseconds timespan(100);
		std::this_thread::sleep_for(timespan);//TO CHECK TODO :voir avec le prof si on peut bien utiliser ça. */
	
	}//TODO : gérer le cas d'abandon d'un joueur
	
	//ici, il y a des joueurs connectés
	//check si ce joueur est invité:
	// dire au client message d'autres joueurs connectés (on entre dans les boucles de connexions)
	
	std::string message6 = "Other players have joined or are available again !";
	const char* buffer6 = message6.c_str();
	size_t buffer_size6 = message6.size(); // should be max 100
	//ssize_t size;
	size = write(f, buffer6, buffer_size6);// send the size of the string so the client can adapt buffer size
	if(size != sizeof(buffer6));
	
	int starting_game = 0;
	while(1){//dans cette boucle tant que on n'est pas dans un jeu
		
		// call check_player_state(player_ptr,socket)
		starting_game = check_player_state(player_ptr,f);
		if(starting_game==2){
			//We entered the game
			break;
		}
		// demander si envoyer invitation(voir liste joueur) ou juste attendre
		//get global player list 
		sem_wait(&global_com_sem);
		std::vector<Player*> global_player_list = global_com_ptr->get_player_list();
		sem_post(&global_com_sem);
		//get available players :
		std::vector<Player*> available_player_list;
		for(int i=0; i<(int)global_player_list.size();i++){
			int player_state = global_player_list[i]->get_state();
			if(player_state==0||player_state==1){
				available_player_list.push_back(global_player_list[i]);
			}
		}
		std::string available_player_name;
		for(int i=0;i< (int)available_player_list.size();i++){
			available_player_name += std::to_string(i) += " : ";
			available_player_name += available_player_list[i]->get_name();
			available_player_name += "\n";
		}
		//send player list to client (avec des numéro)
		int name_list_size = available_player_name.size();
		//send the name_list_size for the buffer to know the length
		int number_available_players = available_player_list.size();
		size = write(f, &number_available_players, sizeof(number_available_players));
		if(size != sizeof(number_available_players));
		
		size = write(f, &name_list_size, sizeof(name_list_size));
		if(size != sizeof(name_list_size));
		
		const char* buffer7 = available_player_name.c_str();
		size_t buffer_size7 = available_player_name.size();
		//ssize_t size;
		size = write(f, buffer7, buffer_size7);
		if(size != sizeof(buffer7));
		
		
		//recevoir la réponse sous forme d'int (-1 si refus et attente)
		int players_choice = 0;
		size = read(f, &players_choice, sizeof(players_choice));
		if(size != sizeof(players_choice));
		
		//call check_player_state(player_ptr,socket)
		starting_game = check_player_state(player_ptr,f);
		if(starting_game==2){
			//We entered the game
			break;
		}
		//if attendre : sleep(5); continue; (on revient au début
		if(players_choice==-1){
			sleep(5);//wait for 5 seconds
		}
		//else if envoyer invit:
			//envoyer l'invitation au joueur sélectionner 
			//envoyer au client le message que l'invitation a bien été envoyé
		else if(players_choice>=0 && players_choice<(int)available_player_list.size()){
			player_ptr->send_invitation(available_player_list[players_choice]);
			std::string message8 = "Your invitation to player "+available_player_list[players_choice]->get_name()+" has been sent";
			
			
			const char* buffer8 = message8.c_str();
			size_t buffer_size8 = message8.size();//has to be <100
			//ssize_t size;
			size = write(f, buffer8, buffer_size8);
			if(size != sizeof(buffer8));
		}
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

	free(thread_param_ptr); //free the memory of parameters pointer
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
	
	// Initialize the semaphore with a value of 1
    	ret = sem_init(&global_com_sem, 0, 1);
	if (ret != 0) {
		fprintf(stderr, "sem_init() failed\n");
		return 0;
	}

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
		Param* thread_param = new Param(&global_com, fd);
		
	        // Acquire the semaphore lock before creating the thread
        	sem_wait(&global_com_sem);
        	
        	pthread_create(&tid, NULL, hconnect, (void *)thread_param);

        	// Release the semaphore lock after creating the thread
        	sem_post(&global_com_sem);
        }

        return 0;
}







// code des règles du jeu 
bool checkWin(char board[3][3]) {
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return true;
        }
    }

    // Check diagonals
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0])) {
        return true;
    }

    return false;
}

bool checkTie(char board[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false;
            }
        }
    }
    return true;
}



// boucle de vérif
void playGame(int clientSocket) {
    char board[3][3] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char message[100];
    int choice;
    int currentPlayer = 0;

    do {
        // Send the current state of the board to clients
        send(clientSocket, board, sizeof(board), 0);

        // Receive the choice from the current player
        recv(clientSocket, &choice, sizeof(choice), 0);

        // Update the board with the current player's move
        char symbol = (currentPlayer == 0) ? 'X' : 'O';
        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (board[row][col] != 'X' && board[row][col] != 'O') {
            board[row][col] = symbol;

            // Check for win condition
            if (checkWin(board)) {
                // Inform clients about win
                sprintf(message, "Player %c wins!\n", symbol);
                send(clientSocket, message, sizeof(message), 0);
                break;
            }

            // Check for tie
            if (checkTie(board)) {
                // Inform clients about tie
                strcpy(message, "It's a tie!\n");
                send(clientSocket, message, sizeof(message), 0);
                break;
            }

            currentPlayer = 1 - currentPlayer; // Switch player
        }
    } while (0/* condition for game continuation */);

    // Send the final state of the board to clients
    send(clientSocket, board, sizeof(board), 0);
}









