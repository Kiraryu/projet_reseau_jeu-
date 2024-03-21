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

	Game(); /* Creates an empty board (9x1 with zeros) and a prefix of length 2 : which contains the player currenctly playing (1 or 2) and the state of play of the current player (0 : hasn't played yet, 1 : has played, 2 : has won the game)*/
	//Game(Player player1, Player player2);
	void assign_player_turn_numbers(Player m_player1, Player m_player2);
	void assign_turn(Player player);
	~Game();
}
