#include "maptile.hpp"

MapTile::MapTile()
{
    m_Character = ' ';
    m_Color = 0;
    m_ColorBright = false;

    //flags
    m_Walkable = true;
    m_Lightable = true;

}
