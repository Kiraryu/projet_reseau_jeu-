
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

int Player::get_socket(){
	return m_socket;
}
int Player::get_state(){
	return m_state;
}
std::string Player::get_name(){
	return m_name;
}
int Player::get_turn_nb(){
	return m_turn_number;
}

std::vector<Player*> Player::get_inviting_players(){
	return m_inviting_players;
}

void Player::invitation_rejected(Player* rejecting_player_id){
	std::cout << "Player" <<  rejecting_player_id->get_name() << " has rejected your invitation." << std::endl;
	m_invited_players.erase(std::remove(m_invited_players.begin(), m_invited_players.end(), rejecting_player_id), m_invited_players.end()); // Removes the rejecting player from the m_invited_players list.
}

void invitation_invalidated(Player* rejecting_player_id); // TODO invalidate the invitations you have SENT to other players

void Player::change_player_turn_number(int new_turn_nb)
{
	m_turn_number = new_turn_nb;   
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
