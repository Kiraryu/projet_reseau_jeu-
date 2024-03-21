#pragma once
#include<string>

#include "Game.h"
#include "Grid.h"

class Player{

private:
	int m_socket;
	std::string m_name;
	int m_state; // 0 : just connected, 1 : has invited or has been invited, 2 : in game.
	std::vector<Player*> m_inviting_players; //list of player that invited this player to play a game
	std::vector<Player*> m_invited_players; // list of the players this player invited to play a game
	
public:
	Player(int socket, std::string name);
	~Player();
	
	void get_invited(Player* player_id); // to be invited by another player
	void change_state(int new_state);
}
