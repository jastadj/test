#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "common.hpp"

class Player
{
private:

    vector2i m_Pos;

    int m_visRadius;

public:
    Player();

    vector2i getPosition() { return m_Pos;}
    void setPosition(int x, int y) { m_Pos.x = x; m_Pos.y = y;}
    void setPosition(vector2i newpos) { m_Pos = newpos;}

    int getVisRadius() { return m_visRadius;}
    void setVisRadius(int nrad) { m_visRadius = nrad;}

};
#endif // CLASS_PLAYER
