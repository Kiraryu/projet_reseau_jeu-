#include "Grid.h"

Grid::Grid()
{
    int m_nb_prefix = 2;
    m_prefix = new int[m_nb_prefix];

    for (int i = 0; i < 9; i++){
        m_positions[i] = 0;
    }
    for (int i = 0; i < m_nb_prefix; i++){
        m_prefix[i] = 0;
    }
}

void Grid::change_prefix(int pos, int value)
{
    if (pos >= 0 && pos < m_nb_prefix) {
        m_prefix[pos] = value;
    } else {
        std::cout << "Error: Index out of bounds. (Index pos out of bounds of prefix length)." << std::endl;
    }
}

void Grid::change_position(int pos, int value)
{
    if (pos >= 0 && pos < 9) {
    	if (m_prefix[pos] != 0){
    		m_positions[pos] = value;
    		m_prefix[2] = 1; // The player has played.
	} else {
		std::cout << "Error: Position already occupied. Try again." << std::endl;
		// TODO : il faut refaire un coup.
	}
    } else {
        std::cout << "Error: Index out of bounds. (Index pos out of bounds of positions length)." << std::endl;
    }
}

Grid::~Grid()
{
    delete[] m_prefix;
}

