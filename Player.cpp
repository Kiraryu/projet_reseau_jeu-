
#include "Player.h"


Player::Player(int socket, std::string name)
{
	m_socket = socket;
	m_name = name;
	m_state = 0; 	// 0 waiting
}


// Copy constructor
Player::Player(const Player& other) {
    // Copy socket, name, and state
    m_socket = other.m_socket;
    m_name = other.m_name;
    m_state = other.m_state;
    // Copy invited players
    for (Player* invited_player : other.m_invited_players) {
        // Create new Player objects and copy their data
        Player* new_invited_player = new Player(*invited_player);
        // Add the copied Player to the m_invited_players list
        m_invited_players.push_back(new_invited_player);
    }
    // Copy inviting players
    for (Player* inviting_player : other.m_inviting_players) {
        // Create new Player objects and copy their data
        Player* new_inviting_player = new Player(*inviting_player);
        // Add the copied Player to the m_inviting_players list
        m_inviting_players.push_back(new_inviting_player);
    }
}

// Destructor
Player::~Player() {
    // Free memory allocated for invited players
    for (Player* invited_player : m_invited_players) {
        delete invited_player;
    }
    // Free memory allocated for inviting players
    for (Player* inviting_player : m_inviting_players) {
        delete inviting_player;
    }
}



void Player::reject_invitation(Player* rejected_player_id){ // reject the invitation you have recieved. Delete rejected_player_id of m_inviting_players de this and call delete_invitation de rejected_player_id (lorsque le client à lu les invitations, s'il en refuse il faut les retirer de notre liste des joueurs invitant, et informer ces joueurs qu'on les a retiré de notre liste pour qu'ils nous retire de leur liste des joueurs invités.)
	auto iterator = std::find(m_inviting_players.begin(), m_inviting_players.end(), rejected_player_id);
	m_inviting_players.erase(iterator);//to delete we need first the position
	//m_inviting_players.erase(std::remove(m_inviting_players.begin(), m_inviting_players.end(), rejected_player_id), m_inviting_players.end()); // Removes the rejected inviting player from the m_inviting_players list.
	std::cout<< "Invit_log : Player " <<  rejected_player_id->get_name() << "'s incitation has been rejected by " << m_name << std::endl;
	rejected_player_id->delete_invitation(this);
}
	
void Player::delete_invitation(Player* refusing_player_id){ // delete the refusing_player_id from m_invited_players of this (lorsqu'on refuse notre invitation, nous fait retirer l'id du joueur de la liste des joueurs qu'on avait invités)
	m_invited_players.erase(std::remove(m_invited_players.begin(), m_invited_players.end(), refusing_player_id), m_invited_players.end()); // Removes the rejecting invited player from the m_invited_players list.
	std::cout<< "Invit_log : The invitation of Player " <<  refusing_player_id->get_name() << " has been removed from the invited_player_list of Player " << m_name << std::endl;
}
	
void Player::invalidate_invitation(Player* rejected_player_id){ // invalidate the invitations you have SENT to other players // call reject_invitation of rejected_player_id with this as argument (demande à un joueur à qui on avait envoyé une invitation de l'invalider en nous la refusant)
	rejected_player_id->reject_invitation(this); // Deletes this players presence from rejected_players inviting_players_list and then calls delete_invitation which deletes the rejected_player from invited_players of this player.
	std::cout<< "Invit_log : The invitation of Player " <<  rejected_player_id->get_name() << " is being invalidated by " << m_name << std::endl;
}
	
void Player::send_invitation(Player* target_player_id){//call target_player_id->receive_invitation(this) if player available, add target_player_id to m_invited_players of this, else do not add the player  (lorsqu'on envoie une invitation, on ajoute le joueur qu'on a invité à la liste des joueurs qu'on a invité, et on demande au joueur de nous ajouter à la liste des gens qui l'ont invités. S'il n'est pas dispo, il refuse directement et on ne l'ajoute pas à la liste)
	int invitation_feedback = target_player_id->receive_invitation(this);
	if (invitation_feedback == 1){
		m_invited_players.push_back(target_player_id);
		std::cout<< "Invit_log : Player " << m_name << " has invited " << target_player_id->get_name() << " to a game."  << std::endl;
	} else {
		// Nothing, the invited player is unavailable.
	}
}
	
int Player::receive_invitation(Player* inviting_player_id){// check the state of this. If 2 return that this is not available. Else, add inviting_player_id to m_inviting_players of this (Lorsqu'on reçoit une invitation, si on est en jeu, on dit qu'on est pas dispo, c'est tout. Si on est dispo, on ajoute la joueur invitant dans la liste des joueurs qui nous invitent)
	if (m_state == 2){
		return 0;
	}
	else if (m_state == 1 || m_state == 0){
		m_inviting_players.push_back(inviting_player_id);
		std::cout<< "Invit_log : Player " << m_name << " has recieved Player " << inviting_player_id->get_name() << "'s invitation."  << std::endl;
		return 1;
	}
	else{
		return -1;//error case, to be shure to return something
	}
}

int Player::get_socket(){
	return m_socket;
}
std::vector<Player*> Player::get_invited_players(){
	return m_invited_players;
}

std::vector<Player*> Player::get_inviting_players(){
	return m_inviting_players;
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
