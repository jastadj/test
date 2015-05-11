#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "common.hpp"
#include <vector>
#include "item.hpp"
#include <string>

class Player
{
private:

    std::string m_Name;

    vector2i m_Pos;
    int m_visRadius;

    std::vector<ItemInstance*> m_Inventory;

    int m_MaxHP;
    int m_CurrentHP;

public:
    Player();
    ~Player();

    std::string getName() { return m_Name;}
    void setName(std::string nname) { m_Name = nname;}

    vector2i getPosition() { return m_Pos;}
    void setPosition(int x, int y) { m_Pos.x = x; m_Pos.y = y;}
    void setPosition(vector2i newpos) { m_Pos = newpos;}
    void setMaxHP(int nmax) { m_MaxHP = nmax;}

    int getVisRadius() { return m_visRadius;}
    void setVisRadius(int nrad) { m_visRadius = nrad;}

    void addItem(ItemInstance *nitem);
    void removeItem(ItemInstance *nitem);
    std::vector<ItemInstance*> *getInventory() { return &m_Inventory;}

    int getAttackDamage() { return 2;}
    int getMaxHP() { return m_MaxHP;}
    int getCurrentHP() { return m_CurrentHP;}
    void addHP(int val);


};
#endif // CLASS_PLAYER
