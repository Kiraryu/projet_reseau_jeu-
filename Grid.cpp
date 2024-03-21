#include "Grid.h"

Grid::Grid()
{
    int m_nb_prefix = 2;
    m_prefix = new int[m_nb_prefix];

    for (int i = 0; i < 9; i++){
        m_placements[i] = 0;
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

Grid::~Grid()
{
    delete[] m_prefix;
}

