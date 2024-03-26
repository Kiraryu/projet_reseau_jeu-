#pragma once
#include "Player.h"
# include <vector>

class Communication {

private :
	std::vector<Player*> players_list;
	//std::vector<Game*> games_list; 

public :
	Communication();
	
	//ask_player_name(); --> voir client
	Player* create_player(int socket, std::string player_name);//return a pointer to the Player instance created, had to store adress of the created player in player list //Done
	start_game(Player player1, Player player2); // changer état des jouers TODO
	std::vector<Player*> get_player_list(); //done return the list of players connected to the server.
};
