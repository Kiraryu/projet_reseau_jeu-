#pragma once
#include "Player.h"
#include "Grid.h"

class Game{
private:
	int game_id;
	Player m_player1;
	Player m_player2;
	Grid m_grid;
	
public:

	Game();
	//Game(Player player1, Player player2);
	~Game();
}
