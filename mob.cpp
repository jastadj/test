#include "mob.hpp"

Mob::Mob(std::string nname, chtype nchar, int ncolor)
{
    //set name
    m_Name = nname;
    //set character icon
    m_Character = nchar;
    //set icon color
    m_Color = ncolor;

    //default AI behavior
    m_DefaultAIBehavior = AI_NONE;

    //set minimum max hp
    m_MaxHP = 1;

}

Mob::~Mob()
{

}

////////////////////////////////////////////////////////////////////
//

MobInstance::MobInstance(Mob *mref, int x, int y)
{
    //link item instance to reference item (master)
    m_MobRef = mref;
    //set item instances position
    m_Position.x = x;
    m_Position.y = y;

    //set default ai behavior
    m_AIBehavior = mref->getDefaultAIBehavior();

    //fill up current hp to max on creation
    m_CurrentHP = getMaxHP();

}

void MobInstance::addHP(int val)
{
    m_CurrentHP += val;

    if(m_CurrentHP > getMaxHP()) m_CurrentHP = getMaxHP();

}
