#include <cstdlib>
#include <ctime>

#include "Game.h"


Game::Game()
{
	std::srand(std::time(nullptr)); // use current time as seed for random generator ***
    	int m_game_id = std::rand();
    	Player m_player1();
    	Player m_player2();
    	Grid m_grid()
}

void Game::assign_player_turn_numbers(Player m_player1, Player m_player2)
{
	m_player1.change_player_turn_number(1);
	m_player2.change_player_turn_number(2);
}

void Game::assign_turn(Player player)
{
	// Modify the player_turn_number.
	m_grid.change_prefix(1, player.get_turn_nb());		// Changes player that has to play next.
	m_grid.change_prefix(2, 0); 		// Changes player state to "hasn't played yet".
}

Game::~Game()
{

}
