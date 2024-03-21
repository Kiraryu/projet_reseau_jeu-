#pragma once
#include<string>

#include "Game.h"
#include "Grid.h"

class Player{

private:
	int m_socket;
	std::string m_name;
	int m_state; // 0 : just connected : 0, has invited or has been invited : 1, in game : 2.
	std::vector<Player*> m_inviting_players; //list of players that invited this player to play a game
	std::vector<Player*> m_invited_players; // list of the players this player invited to play a game
	int m_turn_number(0);
	
public:
	Player(int socket, std::string name);
	~Player();
	
	void send_invitation(Player* sending_player_id); // invitation to play sent by another player who gives his player_id when calling the target's send_invitation method.
	void invitation_rejected(Player* rejecting_player_id); // reject the invitation you have recieved
	void invitation_invalidated(Player* rejecting_player_id); // TODO invalidate the invitations you have SENT to other players
	
	
	void change_state(int new_state);
	void change_player_turn_number(int new_turn_nb);
	
	
	int get_socket();
	int get_state();
	std::string get_name();
	int get_turn_nb();
	
	std::vector<Player*> get_inviting_players();
	
}
