#include "mob.hpp"

Mob::Mob(std::string nname, chtype nchar, int ncolor)
{
    //set name
    m_Name = nname;
    //set character icon
    m_Character = nchar;
    //set icon color
    m_Color = ncolor;

}

Mob::~Mob()
{

}

////////////////////////////////////////////////////////////////////
//
MobInstance::MobInstance(Mob *mref, vector2i ipos)
{
    //link item instance to reference item (master)
    m_MobRef = mref;
    //set item instances position
    m_Position = ipos;
}

MobInstance::MobInstance(Mob *mref, int x, int y)
{
    //link item instance to reference item (master)
    m_MobRef = mref;
    //set item instances position
    m_Position.x = x;
    m_Position.y = y;
}
