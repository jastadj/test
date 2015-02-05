#ifndef CLASS_MOB
#define CLASS_MOB

#include <curses.h>
#include "common.hpp"
#include <string>

class Mob
{
private:

    std::string m_Name;
    chtype m_Character;
    int m_Color;

    //stats
    int m_MaxHP;

public:
    Mob(std::string nname, chtype nchar, int ncolor);
    ~Mob();

    std::string getName() { return m_Name;}
    chtype getCharacter() { return m_Character;}
    int getColor() { return m_Color;}

    int getMaxHP() { return m_MaxHP;}
    void setMaxHP(int nmaxhp) { m_MaxHP = nmaxhp;}
};

class MobInstance
{
private:
    Mob *m_MobRef;

    vector2i m_Position;

    //stats
    int m_CurrentHP;

public:
    MobInstance(Mob *mref, int x, int y);

    vector2i getPosition() { return m_Position;}
    void setPosition(int x, int y) { m_Position.x = x; m_Position.y = y;}
    void setPosition(vector2i npos) { m_Position = npos;}

    std::string getName() { return m_MobRef->getName();}
    chtype getCharacter() { return m_MobRef->getCharacter();}
    int getColor() { return m_MobRef->getColor();}

    int getMaxHP() { return m_MobRef->getMaxHP();}
    int getCurrentHP() { return m_CurrentHP;}
    void addHP(int val);
    bool isDead() { if(m_CurrentHP <= 0) return true; else return false;}

};
#endif // CLASS_MOB
