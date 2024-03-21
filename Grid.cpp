
#include "Grid.h"

Grid::Grid()
{
	int m_nb_prefix = 2;
	m_prefix = new int[m_nb_prefix];
	
	for (int i, i<9, i++){
		m_placements[i] = 0;
	}
	for (int i, i<m_nb_prefix, i++){
		m_prefix[i] = 0;
	}
}



Grid::~Grid()
{
	delete[] m_prefix;	
}
