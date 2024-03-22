
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

void Player::send_invitation(Player* sending_player_id){
	if (m_state == 2){
		sending_player_id->invitation_rejected(this); // Sending notificaiton to the player that invited that he has been rejected
	} else {
		std::cout << "Player" <<  sending_player_id->get_name() << " has invited you to a game." << std::endl;
		std::cout << "Do you wish to accept the invitiation (y/n) ?" << std::endl;
		std::string response;
        	std::cin >> response;
		if(response == "y") {
		    // Store the player_id in the list
		    m_inviting_players.push_back(sending_player_id);
		    // Change the state of the player to 1
		    change_state(1);
		} else if (response == "n") {
		    sending_player_id->invitation_rejected(this); // Sending notification to the player that invited that he has been rejected
		}
	}
	/* TODO :
	- if the state is 2 : refuse invitation and continue the game
	else: 
	- print to the player that it is invited
	- store the player_id (i.e. un pointeur vers l'objet Player) in the list 
	- change the state of the player to 1
	!!! semaphore
	*/
}

void Player::invitation_rejected(Player* rejecting_player_id){
	std::cout << "Player" <<  rejecting_player_id->get_name() << " has rejected your invitation." << std::endl;
	m_invited_players.erase(std::remove(m_invited_players.begin(), m_invited_players.end(), rejecting_player_id), m_invited_players.end()); // Removes the rejecting player from the m_invited_players list.
}

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
