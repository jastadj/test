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

public:
    Player();

    std::string getName() { return m_Name;}
    void setName(std::string nname) { m_Name = nname;}

    vector2i getPosition() { return m_Pos;}
    void setPosition(int x, int y) { m_Pos.x = x; m_Pos.y = y;}
    void setPosition(vector2i newpos) { m_Pos = newpos;}

    int getVisRadius() { return m_visRadius;}
    void setVisRadius(int nrad) { m_visRadius = nrad;}

    void addItem(ItemInstance *nitem);
    void removeItem(ItemInstance *nitem);
    std::vector<ItemInstance*> *getInventory() { return &m_Inventory;}


};
#endif // CLASS_PLAYER
