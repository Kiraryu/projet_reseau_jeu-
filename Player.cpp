
#include "Player.h"


Player::Player(int socket, string name)
{
	m_socket = socket;
	std::string m_name = name;
	int m_state = 0; 	// 0 en attente, 1 peut jouer, 2 attend adversaire
}

Player::~Player()
{

} 			 
