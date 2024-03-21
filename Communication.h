#pragma once

class Communication {

private :
	std::vector<Player*> players_list;
	std::vector<Game*> games_list; 

public :
	Communication();
	
	//ask_player_name(); --> voir client
	void create_player(int socket, std::string player_name);
	send_invite(Player1, Player2);
	start_game(Player player1, Player player2); // changer état des jouers
	std::vector<Player*> get_player_list(); //return the list of players connected to the server.
};