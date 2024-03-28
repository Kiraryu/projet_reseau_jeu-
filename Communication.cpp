# include "Communication.h"


Communication::Communication(){
	
}

Communication::Communication(const Communication& other) {
    // Copy the players list from the other Communication object
    for (Player* player : other.players_list) {
        // Create new Player objects and copy their data
        Player* new_player_ptr = new Player(*player);
        // Add the copied Player to the players list of this Communication object
        players_list.push_back(new_player_ptr);
    }
}

// Destructor
Communication::~Communication() {
    // Free memory allocated for Player objects in the players list
    for (Player* player : players_list) {
        delete player;
    }
}

Player* Communication::create_player(int socket, std::string player_name){
	std::cout << "Creating new player " << player_name << " in socket " << socket << std::endl;
	Player* new_player_ptr = new Player(socket,player_name);//TODO : gérer la destruction de player
	players_list.push_back(new_player_ptr);
	std::cout << "players_list size: " << players_list.size() << " in socket " << socket << std::endl;
	return new_player_ptr;//TODO check with teacher if new_player is not destruct when the function is done 
}

void Communication::start_game(Player player1, Player player2){
	/* Instancier nouveau game, et l'ajouter à games_list
	
	*/
	
	//Game* new_game;
	// Implémenter Sémaphore *** TODO
	//games_list.push_back(new_game);
	
}

std::vector<Player*> Communication::get_player_list(){
	return players_list;
}
