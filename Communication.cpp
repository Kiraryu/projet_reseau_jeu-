# include "Communication.h"
# include <vector>

Communication::Communication(){
	
}

Player* Communication::create_player(int socket, std::string player_name){
	Player* new_player_ptr = new Player(socket,player_name);//TODO : gérer la destruction de player
	players_list.push_back(new_player_ptr);
	return new_player_ptr;//TODO check with teacher if new_player is not destruct when the function is done 
}

Communication::start_game(Player player1, Player player2){
	/* Instancier nouveau game, et l'ajouter à games_list
	
	*/
	
	Game* new_game;
	// Implémenter Sémaphore *** TODO
	games_list.push_back(new_game);
	
}

std::vector<Player*> Communication::get_player_list(){
	return players_list;
}
