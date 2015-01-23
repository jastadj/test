#ifndef CLASS_MAPTILE
#define CLASS_MAPTILE

#include <curses.h>

class MapTile
{
public:
    MapTile();

    chtype m_Character;
    int m_Color;

    //flags
    bool m_Walkable;
    bool m_Lightable;
};

#endif // CLASS_MAPTILE
