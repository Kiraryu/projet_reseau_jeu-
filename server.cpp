#include <string.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "base.h"
#include <cerrno>   // Include errno.h for errno
#include <time.h>
#include <vector>
#include "param.h" //the class to communicate needed parameters to threads
#include <semaphore.h>
#include <fcntl.h>

// Define a semaphore for synchronization
sem_t global_com_sem;

int check_player_state(Player* player_ptr, int socket){
	
	
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
	
	
	while(1) {//s'exécute tant que je suis le seul joueur dispo sur le serveur
		int break_or_not = 0;
		
		player_list = global_com_ptr->get_player_list();
		if(player_list.size() <=1){
			// cout a message to tell that no one else on server
			
		}	
		else{ //TODO :check if that works well (it seems)
			Player* other_player_ptr;
			// other player connected, check if some are available
			for(int i = 0; i<(int)player_list.size();i++){
				//check if it is not us
				other_player_ptr = player_list[i];
				int player_socket = other_player_ptr->get_socket();
				if(player_socket == f){
					continue; //it is us
					}
				int other_player_state = other_player_ptr->get_state();
				//get the state
				//if available break the for and while loop
				if(other_player_state == 0 || other_player_state == 1){
					break_or_not = 1;
					}
							
			} 
			
		}
		if(break_or_not){break;}
		//wait some time
		
		// TODO : maybe put a longer time
		struct timespec sleeping_time;
		sleeping_time.tv_sec = 0;
		sleeping_time.tv_nsec = 100000;
		nanosleep(&sleeping_time, nullptr);
		/*std::chrono::milliseconds timespan(100);
		std::this_thread::sleep_for(timespan);//TO CHECK TODO :voir avec le prof si on peut bien utiliser ça. */
	
	}//TODO : gérer le cas d'abandon d'un joueur, il a juste à fermer son programme, donc il faut vérifier de temps en temps si la connexion est toujours effctive
	
	//ici, il y a des joueurs connectés
	//check si ce joueur est invité:
	// dire au client message d'autres joueurs connectés (on entre dans les boucles de connexions)
	// VERIF : jusque là tout à l'air de bien se passer.
	
	//send the message that other players joined the server
	std::string message6 = "Other players have joined or are available again !";
	
	
	int starting_game = 0;
	while(1){//dans cette boucle tant que on n'est pas dans un jeu
		std::cout << "entered connexion loop on thread linked to socket : " << f << std::endl;
		// call check_player_state(player_ptr,socket)
		starting_game = check_player_state(player_ptr,f);
		if(starting_game==2){
			//We entered the game
			break;
		}
		/* TODO : appeler une fonction qui : 
		- récupère liste des joueur, 
		- la met en forme et l'envoie au client
		- récupère le choix du client
		- renvoie l'adresse du joueur invité (ou nullptr si refus)
		*/
		
		//call check_player_state(player_ptr,socket)
		starting_game = check_player_state(player_ptr,f);
		if(starting_game==2){
			//We entered the game
			break;
		}
		/* TODO 
			appeler une fonction qui récupère l'adresse du joueur invité
			en fonction de si nullptr ou pas :
			attendre 1 secondes
			envoyer les invitations aux autres joueurs.
			envoyer un message au client que ses invitations ont été envoyés
		*/
		
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









