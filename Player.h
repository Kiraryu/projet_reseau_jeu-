#pragma once
#include<string>

#include "Game.h"
#include "Grid.h"

class Player{

private:
	int m_socket;
	std::string m_name;
	int m_state;
	
public:
	Player(int socket, string name);
	~Player();

}
