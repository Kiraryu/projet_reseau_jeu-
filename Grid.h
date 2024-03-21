#pragma once

#include "Game.h"
#include "Player.h"

class Grid{

private:
	int const m_nb_prefix; // nb exact à définir
	int m_positions[9];
	int* m_prefix; 		// Array of size nb_prefix
	
public:
	Grid();
	void change_prefix(int pos, int value);
	void change_position(int pos, int value);
	~Grid();
}
