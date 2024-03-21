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

Game::assign_turn(Player player)
{
	
}

Game::~Game()
{

}
