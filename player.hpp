#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "common.hpp"

class Player
{
private:

    vector2i m_Pos;

public:
    Player();

    vector2i getPosition() { return m_Pos;}
    void setPosition(int x, int y) { m_Pos.x = x; m_Pos.y = y;}
    void setPosition(vector2i newpos) { m_Pos = newpos;}

};
#endif // CLASS_PLAYER
