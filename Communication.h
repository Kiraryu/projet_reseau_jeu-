#pragma once

class Communication {

private :
	std::vector<Player*> players_list;
	std::vector<Game*> games_list; 

public :
	Communication();
	
	ask_player_name();
	create_player();
	send_invite(Player1, Player2);
	start_game(Player player1, Player player2); // changer état des jouers

};
