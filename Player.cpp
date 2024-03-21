
#include "Player.h"


Player::Player(int socket, std::string name)
{
	m_socket = socket;
	std::string m_name = name;
	m_state = 0; 	// 0 waiting
	m_inviting_players = nullptr;
	m_invited_players = nullptr;
}

Player::~Player()
{

} 	

void Player::get_invited(Player* player_id){
	/* TODO :
	- if the state is 2 : refuse invitation and continue the game
	else: 
	- print to the player that it is invited
	- store the player_id in the list 
	- change the state of the player to 1
	!!! semaphore
	*/
}

void Player::change_player_turn_number(int new_turn_nb)
{
	player_turn_number = new_turn_nb;
}


	 
/*
states of a player :
0 waiting (just arriving or after a game ended) 
1 is invited by another player to play a game or sent an invitation to another player to play a game
2 is playing a game
*/
void Player::change_state(int new_state)
{
	m_state = new_state;
}
