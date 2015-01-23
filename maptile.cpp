#include "maptile.hpp"

MapTile::MapTile()
{
    m_Character = ' ';
    m_Color = 0;

    //flags
    m_Walkable = true;
}
